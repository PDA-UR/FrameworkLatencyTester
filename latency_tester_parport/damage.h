#ifndef DAMAGE_H
#define DAMAGE_H

#include <X11/extensions/Xdamage.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
#include <thread>

class DamageHandler {
    private:
        bool running;
        Window win;
        thread measure_xdamage_thread;

    public:
        bool measure;
        bool use_xdamage;
        uint64_t damage_time[1000000];
        uint64_t damage_count;

        DamageHandler(int window_id);
        int handle_xdamage_error(Display *d, XErrorEvent *e);
        void get_xdamage();
        void cleanup();

};

#endif
