#ifndef PARPORT_H
#define PARPORT_H

#include "gpio.h"
#include <thread>

class ParportHandler : public GPIOHandler 
{
    private:
        int fd;
        thread read_thread;
        void readPins();
        bool running;

    public:
        ParportHandler();// : GPIOHandler();
        void cleanup();
};

#endif
