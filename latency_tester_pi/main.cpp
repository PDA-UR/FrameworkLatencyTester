#include <iostream>
#include <chrono>
#include <thread>
#include <string>
//#include <JetsonGPIO.h>
#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <linux/input.h>
#include <cstring>
#include <termios.h>
#include <cerrno>
#include <ctime>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glx.h>
#include <sys/ioctl.h>
#include <X11/extensions/Xdamage.h>

using namespace std;

// Function pointer for glXGetVideoSyncSGI
typedef int (*GLXGETVIDEOSYNCSGIPROC)(unsigned int *);
GLXGETVIDEOSYNCSGIPROC glXGetVideoSyncSGI;

// position and dimension (should only one pixel!) of the region observed by XShm
#define WIDTH 1
#define HEIGHT 1
int X = 200;
int Y = 200;

int ITERATIONS = 100;

// colors used by the test program
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0xFF000000

// input event
#define MOUSE_BUTTON_LEFT 272
#define CLICKED 1

// variables for xshm
Display *display;
Window rootWindow;
XShmSegmentInfo shminfo;
XImage *image;

char* testProgramName;

// Pin Definitions
// wiringPi uses different pin numbers
const int click_pin = 7; //7;
const int bright_pin = 3; //15;
const int bright_pin_2 = 23; //33;

// as printing to stdout is slow, we store all events occuring in a list
// and print its content in the end
#define EVENT_TYPE_CLICK_EVDEV 0
#define EVENT_TYPE_XSHM 1

struct Event {
    uint64_t time;  // event timestamp in microseconds
    int iteration;  // iteration number to identify events belonging together
    int event_type; // used to identify events; replaced with EVENT_NAMES when printing the log
};

// used to make events human readable for easier evaluation
const char* EVENT_NAMES[2] = {"click_evdev", "xshm"};

bool measuring;

uint64_t start_time;
uint64_t click_time;
uint64_t end_time;
uint64_t bright_time;
uint64_t bright_time_2;
uint64_t vsync_time[100000];
uint64_t vsync_count = 0;
uint64_t damage_time[1000000];
uint64_t damage_count = 0;
uint64_t xshm_start_time;
uint64_t xshm_end_time;

bool measure_vblank = 0;
bool measure_xdamage = 0;
bool use_xdamage = 0;

thread fw_test_thread;
thread measure_vblank_thread;
thread measure_xdamage_thread;

Window xdamage_win;

struct termios tty;
int serial_port;
fd_set read_fds;
struct timeval timeout;

unsigned char msg_calibrate[] = {'c'};
unsigned char msg_measure[] = {'m'};
unsigned char msg_toggle[] = {'o'};

// get current microseconds
uint64_t get_micros()
{
    using namespace chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

void initGLX()
{
    Display *glxDisplay = XOpenDisplay(NULL);
    if (!glxDisplay) {
        cout << "Error: Unable to open X display." << endl;
	return;
    }

    int default_screen_id = DefaultScreen(glxDisplay);
    Window glxRoot = RootWindow(glxDisplay, default_screen_id);

    // Create GLX context
    static int visual_attribs[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        None
    };
    XVisualInfo *visual = glXChooseVisual(glxDisplay, default_screen_id, visual_attribs);
    GLXContext context = glXCreateContext(glxDisplay, visual, NULL, GL_TRUE);
    glXMakeCurrent(glxDisplay, glxRoot, context);

    // Load GLX_SGI_video_sync extension
    glXGetVideoSyncSGI = 
        (GLXGETVIDEOSYNCSGIPROC)glXGetProcAddressARB((const GLubyte *)"glXGetVideoSyncSGI");
    if (!glXGetVideoSyncSGI) {
        cout << "Error: GLX_SGI_video_sync extension not supported.\n" << endl;
        return;
    }
}

// initialize the XShm extension to be able to read one pixel from the screen
void initXShm()
{
    XWindowAttributes windowAttributes;
    Screen *screen;

    display = XOpenDisplay(getenv("DISPLAY"));
    rootWindow = DefaultRootWindow(display);
    XGetWindowAttributes(display, rootWindow, &windowAttributes);
    screen = windowAttributes.screen;

    // create an image object that will store the pixel data
    image = XShmCreateImage(display, DefaultVisualOfScreen(screen), DefaultDepthOfScreen(screen), ZPixmap, NULL, &shminfo, WIDTH, HEIGHT);

    shminfo.shmid = shmget(IPC_PRIVATE, image->bytes_per_line * image->height, IPC_CREAT|0777);
    shminfo.shmaddr = image->data = (char*)shmat(shminfo.shmid, 0, 0);
    shminfo.readOnly = False;
    XShmAttach(display, &shminfo);
}

// detach XShm and clean up memory
// this has to be called when the program terminates
void closeXShm()
{
    XShmDetach(display, &shminfo);
    XDestroyImage(image);
}

// get pixel at specified position with XShm
unsigned int getPixelColor()
{
    auto result = XShmGetImage(display, rootWindow, image, X, Y, 0x00ffffff);

    return image->data[2]; // red channel is enough for us
}

unsigned int getPixelColorX()
{
    XColor c;
    XImage *image;

    image = XGetImage (display, rootWindow, X, Y, 1, 1, AllPlanes, XYPixmap);

    c.pixel = XGetPixel (image, 0, 0);

    XFree (image);

    XQueryColor (display, XDefaultColormap(display, XDefaultScreen (display)), &c);

    return c.red / 256;
}

// wait until our pixel has a specified color
void wait_for_color(unsigned int color)
{
	unsigned int pixelColor;
	uint64_t start, end;

	while(1)
	{
		start = get_micros();
		pixelColor = getPixelColor();
		end = get_micros();

		if (pixelColor != 0)
		{
			xshm_start_time = start;
			xshm_end_time = end;
			return;
		}

		usleep(1);
	}
	return;
}

// do all the cleanup before terminating
void cleanup()
{
    measuring = 0;
    close(serial_port);
    closeXShm();
    //GPIO::cleanup();
    fw_test_thread.join();
    measure_vblank_thread.join();
}

// make sure we clean up and print current logs when the program is killed
// log is only printed when terminated, not when interrupted
void signalHandlerInt(int sig)
{
    cleanup();
    exit(sig);
}

void signalHandlerTerm(int sig)
{
    cleanup();
    exit(sig);
}

void trigger_click(void)
{
	click_time = get_micros();
}

void trigger_bright(void)
{
	bright_time = get_micros();
}

void trigger_bright_2(void)
{
	bright_time_2 = get_micros();
}

void measure_fw_latency(int input_fd)
{
    struct input_event inputEvent;
    int err = -1;

    while(measuring)
    {
        // read input events from the specified device
	// take 3 - 4 microseconds
        err = read(input_fd, &inputEvent, sizeof(struct input_event));

        // detected a left mouse click
        if( err > -1 &&
            inputEvent.type == EV_KEY &&
            inputEvent.code == MOUSE_BUTTON_LEFT &&
            inputEvent.value == CLICKED)
        {
            start_time = get_micros();

            wait_for_color(COLOR_WHITE); // wait for test program to react

            end_time = get_micros();
        }
    }
}

int handle_xdamage_error(Display *d, XErrorEvent *e)
{
	use_xdamage = 0;
	return 0;
}

void get_xdamage(Window win)
{
	if (!use_xdamage)
	{
		return;
	}
	//win = 0x3400007;
	//cout << hex << win << endl;

	XSetErrorHandler(handle_xdamage_error);
	Display* dsp = XOpenDisplay(NULL);
	XWindowAttributes attributes = {0};
	int damage_event, damage_error, ret;
	Damage damage;

	try {
		XGetWindowAttributes(dsp, win, &attributes);
		ret = XDamageQueryExtension(dsp, &damage_event, &damage_error);

		if (ret < 1)
		{
			use_xdamage = 0;
			damage_count = 0;
			return;
		}

		damage = XDamageCreate(dsp, win, XDamageReportNonEmpty);
	}
	catch (int e) {
		use_xdamage = 0;
		return;
	}
	XEvent ev;
	//XDamageNotifyEvent* ev_dmg;

	while(measuring)
	{
		if (measure_xdamage)
		{
			XNextEvent(dsp, &ev);
			if (ev.type == damage_event + XDamageNotify)
			{
				//cout << "damage" << endl;
				//ev_dmg = (XDamageNotifyEvent*) &ev;
				uint64_t new_damage_time = get_micros();
				damage_time[damage_count] = new_damage_time;			
				damage_count++;
				if (damage_count > 1000000)
				{
					cout << "obacht! damage_count too big" << endl;
				}
				XDamageSubtract(dsp, damage, None, None);
			}
		}
	}
	XCloseDisplay(dsp);
}

void get_vblanks()
{
	initGLX();

	unsigned int last_sync_count = 0;
	uint64_t last_vblank_time = get_micros();

	while(measuring)
	{
		unsigned int sync_count;
		glXGetVideoSyncSGI(&sync_count);

		if (sync_count != last_sync_count)
		{
			uint64_t new_vblank_time = get_micros();
			last_sync_count = sync_count;

			if (measure_vblank == 1)
			{
				vsync_time[vsync_count] = new_vblank_time;			
				vsync_count++;
			}
		}

		usleep(10);
	}
}

int init_serial_port()
{
	// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
	int port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK); // O_NDELAY

	if (port < 0)
	{
		cout << "Error: could not open serial port." << endl;
	}

	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);

	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

	tty.c_cc[VTIME] = 0;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 2;

	if (tcsetattr(port, TCSANOW, &tty) != 0)
	{
		cout << "Error: could not set tty attributes." << endl;
	}

	return port;
}

int main(int argc, char** argv)
{
    signal(SIGINT, signalHandlerInt);
    signal(SIGTERM, signalHandlerTerm);

    srand(time(nullptr));

    // variables used by evdev to get input events
    int input_fd = -1;
    char* event_handle; // path to /dev/input/eventXY for the used input device

    // check command line parameters
    if(argc < 4)
    {
        cerr << "Too few arguments!" << endl
             << "Usage: latency_tester INPUT_DEVICE PROGRAM_NAME ITERATIONS" << endl
             << "INPUT_DEVICE: path to /dev/input/eventXY" << endl
             << "PROGRAM_NAME: name of program under test to appear in log files" << endl
             << "ITERATIONS: number of iterations" << endl;
        exit(SIGABRT);
    }
    else
    {
        event_handle = argv[1];
        testProgramName = argv[2];
	ITERATIONS = atoi(argv[3]);
    }

    if(argc == 5)
    {
        xdamage_win = atoi(argv[4]);
	use_xdamage = 1;
    }

    int iteration = 0;
    timeout.tv_sec = 0;
    timeout.tv_usec = 200000;
    FD_ZERO(&read_fds);
    FD_SET(serial_port, &read_fds);

    initXShm();

    // open input device
    input_fd = open(event_handle, O_RDONLY | O_NONBLOCK);

    if(input_fd == -1)
    {
        cerr << "Could not open input device " << event_handle << endl;
        exit(SIGABRT);
    }

    // Pin Setup.
    //wiringPiSetupGpio();
    wiringPiSetup();

    // set pin as an output pin with optional initial state of HIGH
    //pinMode(click_pin, INPUT);
    //pinMode(bright_pin, INPUT);
    //pinMode(bright_pin_2, INPUT);

    wiringPiISR(click_pin, INT_EDGE_RISING, &trigger_click); // , 10)
    wiringPiISR(bright_pin, INT_EDGE_RISING, &trigger_bright);
    wiringPiISR(bright_pin_2, INT_EDGE_RISING, &trigger_bright_2);

    serial_port = init_serial_port();

    if (serial_port < 0)
    {
	    cleanup();
	    exit(SIGABRT);
    }

    char serial_read_buffer[256];
    int serial_read_num_bytes = 0;

    usleep(2 * 1000 * 1000);
    write(serial_port, msg_toggle, 1); 
    usleep(500 * 1000);
    write(serial_port, msg_toggle, 1);
    usleep(500 * 1000);

    write(serial_port, msg_calibrate, 1);
    //memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));

    // TODO: fix this using poll() or select()
    memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));
    serial_read_num_bytes = 0;

    usleep(2 * 1000 * 1000);

    serial_read_num_bytes = read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer));
    //cout << "calib: " << serial_read_buffer << endl;

    usleep(1000);
    ioctl(serial_port, TCFLSH, 2);
    usleep(1000);
    
    measuring = 1;

    fw_test_thread = thread(measure_fw_latency, input_fd);

    usleep(100 * 1000);

    measure_vblank = 0;
    measure_vblank_thread = thread(get_vblanks);

    if (use_xdamage)
    {
	    measure_xdamage = 0;
	    measure_xdamage_thread = thread(get_xdamage, xdamage_win);
    }

    usleep(100 * 1000);

    cout << "iteration,click_time,start_time,end_time,bright_time,bright_time_2,xshm_start_time,xshm_end_time,yalmd_latency,vblanks,damage" << endl;

    while(measuring)
    {
	    start_time = 0;
	    click_time = 0;
	    end_time = 0;
	    bright_time = 0;
	    bright_time_2 = 0;

	    measure_vblank = 1;
	    measure_xdamage = 1;
	    
	    usleep(20000);

	    write(serial_port, msg_measure, 1);

	    while (start_time == 0 || click_time == 0 || end_time == 0 || bright_time == 0 || bright_time_2 == 0)
	    {
		usleep(10);
	    }

	    usleep(20000);

	    measure_vblank = 0;
	    measure_xdamage = 0;

	    serial_read_num_bytes = 0;
	    memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));

	    usleep(300000);

	    serial_read_num_bytes = read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer));

	    int input_latency = start_time - click_time;
	    int framework_latency = end_time - start_time;
	    int display_latency = bright_time - end_time;
	    int ete_latency = bright_time - click_time;
	    int sum_latency = input_latency + framework_latency + display_latency;
	    int yalmd_latency = atoi(serial_read_buffer);

	    //cout << "diff: " << (int)(bright_time_2 - bright_time) << endl;

	    cout << iteration << ","
		 << click_time << ","
		 << start_time << ","
		 << end_time << ","
		 << bright_time << ","
		 << bright_time_2 << ","
		 << xshm_start_time << ","
		 << xshm_end_time << ","
		 << yalmd_latency << ",";

		for (int i = 0; i < vsync_count; i++)
		{
			cout << vsync_time[i] << ";";
			vsync_time[i] = 0;
		}

		cout << ",";

		if (use_xdamage)
		{
			for (int i = 0; i < damage_count; i++)
			{
				cout << damage_time[i] << ";";
				damage_time[i] = 0;
			}
		}

		cout << endl;

		vsync_count = 0;
		damage_count = 0;

	    iteration++;

	    if (iteration > ITERATIONS) break;

	    usleep(200 * 1000);
    }

    cleanup();

    return 0;
}
