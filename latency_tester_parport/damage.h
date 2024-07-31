#ifndef DAMAGE_H
#define DAMAGE_H

#include <X11/extensions/Xdamage.h>

extern uint64_t damage_time[1000000];
extern uint64_t damage_count;
extern bool measure_xdamage;
extern bool use_xdamage;
extern Window xdamage_win;
extern thread measure_xdamage_thread;

void get_xdamage(Window win);
int handle_xdamage_error(Display *d, XErrorEvent *e);

#endif
