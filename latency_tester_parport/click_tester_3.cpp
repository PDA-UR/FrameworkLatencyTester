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
#include <linux/input.h>
#include <cstring>
#include <termios.h>
#include <cerrno>
#include <ctime>
#include <cstdlib>

using namespace std;

// input event
#define MOUSE_BUTTON_LEFT 272
#define CLICKED 1

char* testProgramName;

// Pin Definitions
const int click_pin = 7;
const int bright_pin = 15;

bool measuring;
bool was_bright = 1;

uint64_t start_time;
uint64_t end_time;

struct termios tty;
int serial_port;

unsigned char msg_calibrate[] = {'c'};
unsigned char msg_measure[] = {'m'};
unsigned char msg_toggle[] = {'o'};
unsigned char msg_test[] = {'t'};

// get current microseconds
uint64_t micros()
{
    using namespace chrono;
    //return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

// do all the cleanup before terminating
void cleanup()
{
    measuring = 0;
    close(serial_port);
    GPIO::cleanup();
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
	start_time = micros();
}

void trigger_bright(const std::string& channel)
{
	//cout << "BRIGHT!" << endl;
	was_bright = 1;
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
    if(argc < 3)
    {
        cerr << "Too few arguments!" << endl
             << "Usage: latency_tester INPUT_DEVICE PROGRAM_NAME" << endl
             << "INPUT_DEVICE: path to /dev/input/eventXY" << endl
             << "PROGRAM_NAME: name of program under test to appear in log files" << endl;
        exit(SIGABRT);
    }
    else
    {
        event_handle = argv[1];
        testProgramName = argv[2];
    }

    int iteration = 0;

    // Pin Setup.
    GPIO::setmode(GPIO::BOARD);

    // set pin as an output pin with optional initial state of HIGH
    GPIO::setup(click_pin, GPIO::IN);
    GPIO::setup(bright_pin, GPIO::IN);

    GPIO::add_event_detect(click_pin, GPIO::Edge::RISING, trigger_click, 0); // , 10)
    GPIO::add_event_detect(bright_pin, GPIO::Edge::RISING, trigger_bright, 0); // , 10)

    serial_port = init_serial_port();

    if (serial_port < 0)
    {
	    cleanup();
	    exit(SIGABRT);
    }

    char serial_read_buffer[128];
    int serial_read_num_bytes = 0;
    
    measuring = 1;

    //cout << "iteration,click_time,start_time,end_time,bright_time,yalmd_latency" << endl;
    cout << "iteration,latency" << endl;

    struct input_event inputEvent;
    int err = -1;

    usleep(3000 * 1000);

	write(serial_port, msg_calibrate, 1);


    // open input device
    input_fd = open(event_handle, O_RDONLY | O_NONBLOCK);

    if(input_fd == -1)
    {
        cerr << "Could not open input device " << event_handle << endl;
        exit(SIGABRT);
    }

    usleep(3000 * 1000);

    while(measuring)
    {
	    was_bright = 0;
	    start_time = 0;
	    end_time = 0;
	    //cout << "measure" << endl;
	    write(serial_port, msg_measure, 1);

	    while(!start_time)
	    {
		usleep(10);
	    }

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
			end_time = micros();
			cout << iteration << "," << (end_time - start_time) / 1000.0f  << "," << start_time  << "," << end_time << endl;
			iteration++;
			usleep(500 * 1000);
			break;
		}
	    }

	    //cout << "before was bright" << endl;
	    while(!was_bright)
	    {
		usleep(10);
	    }

	    //cout << "was bright" << endl;
			//write(serial_port, msg_toggle, 1);

	    usleep((rand() / RAND_MAX) * 1000 * 1000);
    }

    //printLog();
    cleanup();

    return 0;
}
