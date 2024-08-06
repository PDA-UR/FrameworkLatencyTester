#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>

using namespace std;
const unsigned char msg_calibrate[] = {'c'};
const unsigned char msg_measure[] = {'m'};
const unsigned char msg_toggle[] = {'o'};


class SerialHandler() {
    private:
        struct termios tty;
        int serial_port;
        fd_set read_fds;

    public:
        bool initialized;

        SerialHandler();
        void write(char* msg, int length);
        void flush();
        string read();
        int readInt();

        void cleanup();

}

#endif
