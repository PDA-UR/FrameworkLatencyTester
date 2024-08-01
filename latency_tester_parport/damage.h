#ifndef DAMAGE_H
#define DAMAGE_H

#include <X11/extensions/Xdamage.h>

class DamageHandler {
    private:
        uint64_t damage_time[1000000];
        uint64_t damage_count;
        bool measure_xdamage;
        bool use_xdamage;
        Window xdamage_win;
        thread measure_xdamage_thread;
        int handle_xdamage_error(Display *d, XErrorEvent *e);

    public:
        DamageHandler();
        void get_xdamage(Window win);

}

#endif
