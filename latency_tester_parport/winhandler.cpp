#include "main.h"
#include "inputhandler.h"
#include "winhandler.h"

#include <windows.h>

using namespace std;

WinHandler::WinHandler(char *path) : InputHandler()
{
    state = GetKeyState(VK_LBUTTON);
}

void WinHandler::handle_input()
{
    while (running)
    {
        // check current state of left mouse key
        SHORT currentState = GetKeyState(VK_LBUTTON);

        // detected a left mouse click
        if (currentState != state && currentState < 0)
        {
            input_time = get_micros();
            notify_callbacks();
            state = currentState;
        }
    }
}
