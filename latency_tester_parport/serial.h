#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>
#include <string>

using namespace std;
const unsigned char msg_calibrate[] = {'c'};
const unsigned char msg_measure[] = {'m'};
const unsigned char msg_toggle[] = {'o'};


class SerialHandler {
    private:
        struct termios tty;
        int serial_port;
        //fd_set read_fds;

    public:
        bool initialized;

        SerialHandler(const char* path);
        void writeMessage(char* msg, int length);
        void flush();
        string readString();
        int readInt();

        void cleanup();

};

#endif
