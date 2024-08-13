#include "main.h"
#include "inputhandler.h"
#include "evdevhandler.h"

#include <cstdlib>
#include <linux/input.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

EvdevHandler::EvdevHandler(char* path) : InputHandler()
{
    // open input device
    input_fd = open(path, O_RDONLY | O_NONBLOCK);

    // todo this should be an exception
    if(input_fd == -1)
    {
        cerr << "Could not open input device " << path << endl;
        exit(SIGABRT);
    }
    //cout << "init evdev handler" << endl;
}

void EvdevHandler::handle_input()
{
    struct input_event inputEvent;
    int err = -1;

    while(running)
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
            input_time = get_micros();
	    //cout << "notify from evdev" << endl;
            notify_callbacks();
        }
    }
}
