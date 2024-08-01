#ifndef PARPORT_H
#define PARPORT_H

#include <linux/ppdev.h>
#include <linux/parport.h>

// Pin Definitions
// wiringPi uses different pin numbers
const int click_pin = 5; //7;
const int bright_pin = 4; //15;
const int bright_pin_2 = 3; //33;

extern bool parport_active;
extern int parportfd;
extern thread read_parport_thread;

void init_parport();
void read_partport();
void cleanup_parport();

#endif
