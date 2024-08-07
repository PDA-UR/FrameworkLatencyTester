#include "main.h"
#include "controller.h"
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int ITERATIONS = 100;

MeasurementController *measurementController;

int main(int argc, char** argv)
{
    signal(SIGINT, signalHandlerInt);
    signal(SIGTERM, signalHandlerTerm);
    
    srand(time(nullptr));

    char* event_handle; // path to /dev/input/eventXY for the used input device

    int damage_win = 0;

    bool use_xdamage = false;

    char* testProgramName;

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
        damage_win = atoi(argv[4]);
	    use_xdamage = 1;
    }

    // should this be after opening the serial port?
    //FD_ZERO(&read_fds);
    //FD_SET(serial_port, &read_fds);

    measurementController = new MeasurementController(event_handle, damage_win, ITERATIONS);

    measurementController->run();

    return 0;
}

void signalHandlerInt(int sig)
{
    measurementController->cleanup();
    exit(sig);
}

void signalHandlerTerm(int sig)
{
    measurementController->cleanup();
    exit(sig);
}
