#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>

const unsigned char msg_calibrate[] = {'c'};
const unsigned char msg_measure[] = {'m'};
const unsigned char msg_toggle[] = {'o'};

extern struct termios tty;
extern int serial_port;
extern fd_set read_fds;

int init_serial_port();

#endif
