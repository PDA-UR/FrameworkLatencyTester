#ifndef VBLANK_H
#define VBLANK_H

#include <GL/gl.h>
#include <GL/glx.h>

extern uint64_t vsync_time[100000];
extern uint64_t vsync_count = 0;
extern bool measure_vblank;
extern thread measure_vblank_thread;

// Function pointer for glXGetVideoSyncSGI
typedef int (*GLXGETVIDEOSYNCSGIPROC)(unsigned int *);
extern GLXGETVIDEOSYNCSGIPROC glXGetVideoSyncSGI;

void initGLX();
void get_vblanks();

#endif
