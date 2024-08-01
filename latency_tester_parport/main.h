#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <linux/input.h>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <cstdlib>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>

// used by exec
#include <cstdarg>
#include <fstream>
#include <memory>
#include <cstdio>

// input event
#define MOUSE_BUTTON_LEFT 272
#define CLICKED 1

using namespace std;

extern char* testProgramName;
extern int ITERATIONS;
extern bool state_click;
extern bool state_bright;
extern bool state_bright_2;

extern bool measuring;

extern uint64_t start_time;
extern uint64_t click_time;
extern uint64_t end_time;
extern uint64_t bright_time;
extern uint64_t bright_time_2;

extern thread fw_test_thread;

extern struct timeval timeout;

uint64_t get_micros();
void cleanup();
void signalHandlerInt(int sig);
void signalHandlerTerm(int sig);
void trigger_click(void);
void trigger_bright(void);
void trigger_bright_2(void);

string exec(const char* cmd);

#endif
