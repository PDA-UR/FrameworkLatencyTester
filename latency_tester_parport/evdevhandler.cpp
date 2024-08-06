#include "inputhandler.h"
#include "evdevhandler.h"

using namespace std;

EvdevHandler::EvdevHandler(char* path)
{
    // open input device
    input_fd = open(path, O_RDONLY | O_NONBLOCK);

    // todo this should be an exception
    if(input_fd == -1)
    {
        cerr << "Could not open input device " << event_handle << endl;
        exit(SIGABRT);
    }
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
            notify_callbacks();
        }
    }
}
