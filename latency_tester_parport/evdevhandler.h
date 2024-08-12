#ifndef EVDEV_HANDLER_H
#define EVDEV_HANDLER_H

class EvdevHandler : public InputHandler
{
    private:
        int input_fd;

    public:
        EvdevHandler(char* path);// : InputHandler();
        void handle_input() override;

};

#endif
