#ifndef VBLANK_H
#define VBLANK_H

#include <GL/gl.h>
#include <GL/glx.h>

// Function pointer for glXGetVideoSyncSGI
typedef int (*GLXGETVIDEOSYNCSGIPROC)(unsigned int *);

class VblankHandler {
    private:
        GLXGETVIDEOSYNCSGIPROC glXGetVideoSyncSGI;
        uint64_t vsync_time[100000];
        uint64_t vsync_count;
        bool measure_vblank;
        thread measure_vblank_thread;

    public:
        VblankHandler();
        void get_vblanks();
}

#endif
