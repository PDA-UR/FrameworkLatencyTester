#ifndef VBLANK_H
#define VBLANK_H

#include <GL/gl.h>
#include <GL/glx.h>
#include <thread>

// Function pointer for glXGetVideoSyncSGI
typedef int (*GLXGETVIDEOSYNCSGIPROC)(unsigned int *);

class VblankHandler {
    private:
        GLXGETVIDEOSYNCSGIPROC glXGetVideoSyncSGI;
        bool measure_vblank;
        bool running;
        thread measure_vblank_thread;

    public:
        uint64_t vsync_time[100000];
        uint64_t vsync_count;

        VblankHandler();
        bool measure;
        void get_vblanks();
        void cleanup();
};

#endif
