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

using namespace std;

// input event
#define MOUSE_BUTTON_LEFT 272
#define CLICKED 1

char* testProgramName;

// Pin Definitions
const int click_pin = 7;

bool measuring;

// get current microseconds
uint64_t micros()
{
    using namespace chrono;
    return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
    //return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
    //return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

// do all the cleanup before terminating
void cleanup()
{
    measuring = 0;
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
    GPIO::setup(click_pin, GPIO::OUT, GPIO::LOW);

    
    measuring = 1;

    //cout << "iteration,click_time,start_time,end_time,bright_time,yalmd_latency" << endl;
    //
    cout << "iteration,latency,time" << endl;


    struct input_event inputEvent;
    int err = -1;

    uint64_t start_time;
    uint64_t end_time;

    int stage = 0;

    while(measuring)
    {
	    GPIO::output(click_pin, GPIO::HIGH);
	    start_time = micros();

	    while(measuring)
	    {
		// read input events from the specified device
		// take 3 - 4 microseconds
		//
		//uint64_t read_start = micros();
		//
		err = read(input_fd, &inputEvent, sizeof(struct input_event));

		//uint64_t read_end = micros();
		//uint64_t read_delta = read_end - read_start;
		//if (read_delta > 1000)
		//{
		//	cout << read_delta / 1000.0f << " - " << err << endl;
		//}

		// detected a left mouse click
		if( err > -1 &&
		    inputEvent.type == EV_KEY &&
		    inputEvent.code == MOUSE_BUTTON_LEFT &&
		    inputEvent.value == CLICKED)
		{
		    end_time = micros();
	    	    GPIO::output(click_pin, GPIO::LOW);
		    cout << iteration << "," << (end_time - start_time) / 1000.0f << "," << start_time << endl;
		    iteration++;

		    //if (iteration > (stage * 1000) + 1000) stage++;

		    //usleep(10000 + stage * 1000);
		    //usleep(30000 + iteration * 10);

		    //usleep(500 * 1000);

			// wait for release
			// TODO
			while (1)
			{
				err = read(input_fd, &inputEvent, sizeof(struct input_event));
				if( err > -1 &&
				    inputEvent.type == EV_KEY &&
				    inputEvent.code == MOUSE_BUTTON_LEFT &&
				    inputEvent.value == 0)
				{
					break;
				}
				usleep(10);
			}
			break;
		}

		usleep(50);

	    }

	    //usleep((rand() / RAND_MAX) * 1000 * 1000);
	    //usleep(10000 + (rand() / RAND_MAX) * (1000000 - 100));
	    //usleep(100 + (((double)rand()) / RAND_MAX) * (10000 - 100));
	    usleep(100 + rand() % (10000 - 100));
	    //usleep(rand() % (1000000));
	    //sleep(rand() % 2);
    }

    //printLog();
    cleanup();

    return 0;
}
