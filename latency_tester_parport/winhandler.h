#ifndef WIN_HANDLER_H
#define WIN_HANDLER_H

#include "inputhandler.h"
#include <windows.h>

class WinHandler : public InputHandler
{
private:
    int input_fd;
    SHORT state;

public:
    WinHandler(char *path); // : InputHandler();
    void handle_input();
};

#endif