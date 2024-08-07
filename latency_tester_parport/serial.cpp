#include "main.h"
#include "serial.h"

#include <string>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

SerialHandler::SerialHandler(const char* path)
{
	// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
	int port = open(path, O_RDWR | O_NOCTTY | O_NONBLOCK); // O_NDELAY

	if (port < 0)
	{
		cout << "Error: could not open serial port." << endl;
	}

	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);

	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	/*
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

	tty.c_cc[VTIME] = 0;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 2;
	//tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	//tty.c_cc[VMIN] = 0;
	*/

	if (tcsetattr(port, TCSANOW, &tty) != 0)
	{
		cout << "Error: could not set tty attributes." << endl;
	}

	//return port;
    serial_port = port;

    if (port >= 0) initialized = true;
}

void SerialHandler::writeMessage(char* msg, int length)
{
    write(serial_port, msg, length);
}

string SerialHandler::readString()
{
    char serial_read_buffer[256];
    int serial_read_num_bytes = 0;

    // TODO: fix this using poll() or select()
    memset(&serial_read_buffer, '\0', sizeof(serial_read_buffer));
    serial_read_num_bytes = 0;

    serial_read_num_bytes = read(serial_port, &serial_read_buffer, sizeof(serial_read_buffer));
    
    return str(serial_read_buffer);
}

int SerialHandler::readInt()
{
    return stoi(readString());
}

void SerialHandler::flush()
{
    int tcflush_result = tcflush(serial_port, TCIOFLUSH);
    //cout << "tcflush says " << tcflush_result << endl;
}

void SerialHandler::cleanup()
{
    close(serial_port);
}
