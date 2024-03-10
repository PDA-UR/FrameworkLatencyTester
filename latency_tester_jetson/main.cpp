#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <JetsonGPIO.h>
#include <stdio.h>
#include <vector>
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

using namespace std;

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
const int click_pin = 7;
const int bright_pin = 15;

// as printing to stdout is slow, we store all events occuring in a list
// and print its content in the end
#define EVENT_TYPE_CLICK_EVDEV 0
#define EVENT_TYPE_XSHM 1

struct Event {
    uint64_t time;  // event timestamp in microseconds
    int iteration;  // iteration number to identify events belonging together
    int event_type; // used to identify events; replaced with EVENT_NAMES when printing the log
};

vector<Event> events;   // event list

// used to make events human readable for easier evaluation
const char* EVENT_NAMES[2] = {"click_evdev", "xshm"};

bool measuring;

uint64_t start_time;
uint64_t click_time;
uint64_t end_time;
uint64_t bright_time;

thread fw_test_thread;

struct termios tty;
int serial_port;

unsigned char msg_calibrate[] = {'c'};
unsigned char msg_measure[] = {'m'};
unsigned char msg_toggle[] = {'o'};

// appends event to the events list
void logEvent(uint64_t time, int event, int iteration)
{
    Event tempEvent = Event();
    tempEvent.time = time;
    tempEvent.event_type = event;
    tempEvent.iteration = iteration;
    events.push_back(tempEvent);
}

// called when program is finished
// iterates the events list and prints it to stdout in CSV format
void printLog()
{
    cout << "time,iteration,event,program" << endl;
    for(auto const& ev : events)
    {
        cout << dec <<
            ev.time << "," <<
            ev.iteration << "," <<
            EVENT_NAMES[ev.event_type] << "," <<
            testProgramName << endl;
    }
}

// get current microseconds
uint64_t micros()
{
    using namespace chrono;
    //return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
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

    //cout << "shmid: " << shminfo.shmid << endl;
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
    //uint64_t start_time = micros();

    //cout << "get image" << endl;
    auto result = XShmGetImage(display, rootWindow, image, X, Y, 0x00ffffff);
    //cout << "result " << result << endl;
    //cout << "got image " << dec << image->data[2] << " yay" << endl;

    //uint64_t end_time = micros();
    //cout << end_time - start_time << endl;

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

    //cout << c.red/256 << " " << c.green/256 << " " << c.blue/256 << "\n";
    //cout << c.red / 256 << endl;

    return c.red / 256;
}

// wait until our pixel has a specified color
void wait_for_color(unsigned int color)
{
    //cout << getPixelColorX() << " " << color << endl;
    while(getPixelColor() == 0) // != color
    //while(getPixelColorX() != color)
    {
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
    GPIO::cleanup();
    fw_test_thread.join();
}

// make sure we clean up and print current logs when the program is killed
// log is only printed when terminated, not when interrupted
void signalHandlerInt(int sig)
{
    //printLog();
    cleanup();
    exit(sig);
}

void signalHandlerTerm(int sig)
{
    //printLog();
    cleanup();
    exit(sig);
}

void trigger_click(const std::string& channel)
{
	click_time = micros();
}

void trigger_bright(const std::string& channel)
{
	bright_time = micros();
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

        //uint64_t before = micros();
        //uint64_t after = micros();
	//float delta = (after - before) / 1000.0f;
	//if (delta != 0) cout << delta << endl;

        // detected a left mouse click
        if( err > -1 &&
            inputEvent.type == EV_KEY &&
            inputEvent.code == MOUSE_BUTTON_LEFT &&
            inputEvent.value == CLICKED)
        {
            start_time = micros();
            //logEvent(micros(), EVENT_TYPE_CLICK_EVDEV, iteration); // log input event timestamp
            wait_for_color(COLOR_WHITE); // wait for test program to react
            //wait_for_color(255); // wait for test program to react

            //logEvent(micros(), EVENT_TYPE_XSHM, iteration); // log color change timestamp
            end_time = micros();

            //cout << end_time - start_time << endl;
            //iteration++;
        }

        // does not seem to change anything
        //usleep(10);
    }
}

int init_serial_port()
{
	// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
	int port = open("/dev/ttyUSB0", O_RDWR);

	if (port < 0)
	{
		cout << "Error: could not open serial port." << endl;
	}

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

    if(argc == 6)
    {
        X = atoi(argv[4]);
        Y = atoi(argv[5]);
    }

    int iteration = 0;

    //cout << "init XShm" << endl;
    initXShm();

    //while(true)
    //{
    //        logEvent(micros(), EVENT_TYPE_CLICK_EVDEV, iteration); // log input event timestamp
    //        wait_for_color(COLOR_WHITE); // wait for test program to react
    //        logEvent(micros(), EVENT_TYPE_XSHM, iteration); // log color change timestamp
    //        iteration++;
    //}

    // open input device
    input_fd = open(event_handle, O_RDONLY | O_NONBLOCK);

    if(input_fd == -1)
    {
        cerr << "Could not open input device " << event_handle << endl;
        exit(SIGABRT);
    }

    // Pin Setup.
    GPIO::setmode(GPIO::BOARD);

    // set pin as an output pin with optional initial state of HIGH
    GPIO::setup(click_pin, GPIO::IN);
    GPIO::setup(bright_pin, GPIO::IN);

    GPIO::add_event_detect(click_pin, GPIO::Edge::RISING, trigger_click, 0); // , 10)
    GPIO::add_event_detect(bright_pin, GPIO::Edge::RISING, trigger_bright, 0);

    serial_port = init_serial_port();

    if (serial_port < 0)
    {
	    cleanup();
	    exit(SIGABRT);
    }

    char serial_read_buffer[128];
    int serial_read_num_bytes = 0;

    usleep(3 * 1000 * 1000);

    write(serial_port, msg_calibrate, 1);
    memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));

    //cout << "buffer size" << sizeof(serial_read_buffer) << endl;

    do {
    	serial_read_num_bytes = read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer));
	//cout << serial_read_buffer;
    } while (serial_read_num_bytes <= 0);

    // TODO: fix this using poll() or select()
    memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));
    serial_read_num_bytes = 0;
    do {
    	serial_read_num_bytes = read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer));
	//cout << serial_read_buffer;
    } while (serial_read_num_bytes <= 0);

    //cout << "num bytes received: " << serial_read_num_bytes << endl;
    //cout << "calibration: " << serial_read_buffer << endl;
    //cout << endl << "calibration finished" << endl;

    usleep(2 * 1000 * 1000);
    
    measuring = 1;

    fw_test_thread = thread(measure_fw_latency, input_fd);

    cout << "iteration,click_time,start_time,end_time,bright_time,yalmd_latency" << endl;

    while(measuring)
    {
	    start_time = 0;
	    click_time = 0;
	    end_time = 0;
	    bright_time = 0;

	    write(serial_port, msg_measure, 1);


	    while (start_time == 0 || click_time == 0 || end_time == 0 || bright_time == 0)
	    {
	    	//cout << click_time << "," << start_time << "," << end_time << "," << bright_time << endl;
		usleep(10);
	    }
	    //cout << click_time << "," << start_time << "," << end_time << "," << bright_time << endl;
	    
	    serial_read_num_bytes = 0;
	    memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));
	    do {
		    //cout << ".";
	            //cout << click_time << "," << start_time << "," << end_time << "," << bright_time << endl;
		    serial_read_num_bytes = read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer));
	    } while (serial_read_num_bytes == 0);

	    int input_latency = start_time - click_time;
	    int framework_latency = end_time - start_time;
	    int display_latency = bright_time - end_time;
	    int ete_latency = bright_time - click_time;
	    int sum_latency = input_latency + framework_latency + display_latency;
	    int yalmd_latency = atoi(serial_read_buffer);

	    cout << iteration << ","
		 << click_time << ","
		 << start_time << ","
		 << end_time << ","
		 << bright_time << ","
		 << yalmd_latency
		 << endl;

	    iteration++;

	    if (iteration > ITERATIONS) break;
	    /*
	    //cout << "read is non-blocking" << endl;
	    //cout << click_time << "," << start_time << "," << end_time << "," << bright_time << endl;
	    cout << "-------------" << endl;
	    cout << "input latency:     " << input_latency << endl;
	    cout << "framework latency: " << framework_latency << endl;
	    cout << "display latency:   " << display_latency << endl;
	    cout << "ete latency:       " << ete_latency << endl;
	    //cout << "sum latency:       " << sum_latency << endl;
	    cout << "yalmd:             " << yalmd_latency << endl;
	    cout << "error:             " << ete_latency - yalmd_latency << endl;
	    */

	    //usleep((rand() / RAND_MAX) * 1000 * 1000);
	    usleep(200 * 1000);
    }

    //printLog();
    cleanup();

    return 0;
}
