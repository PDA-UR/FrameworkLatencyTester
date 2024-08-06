#ifndef DAMAGE_H
#define DAMAGE_H

#include <X11/extensions/Xdamage.h>

class DamageHandler {
    private:
        uint64_t damage_time[1000000];
        uint64_t damage_count;
        bool running;
        Window win;
        thread measure_xdamage_thread;
        int handle_xdamage_error(Display *d, XErrorEvent *e);

    public:
        bool measure;
        bool use_xdamage;
        DamageHandler(Window window_id);
        void get_xdamage();
        void cleanup();

}

#endif
