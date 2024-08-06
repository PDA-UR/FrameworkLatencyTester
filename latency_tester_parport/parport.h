#ifndef PARPORT_H
#define PARPORT_H

class ParportHandler : GPIOHandler {
    private:
        int fd;
        void trigger_click();
        void trigger_bright();
        void trigger_bright_2();
        thread read_thread;
        void read();
        bool running;

    public:
        ParportHandler() : GPIOHandler();
}

#endif
