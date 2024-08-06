#ifndef EVDEV_HANDLER_H
#define EVDEV_HANDLER_H

class EvdevHandler : InputHandler
{
    private:
        input_fd;

    public:
        EvdevHandler(char* path) : InputHandler();

}

#endif
