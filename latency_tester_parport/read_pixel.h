#ifndef READ_PIXEL_H
#define READ_PIXEL_H

#include <sys/shm.h>
#include <X11/extensions/XShm.h>

// position and dimension (should only one pixel!) of the region observed by XShm
#define WIDTH 1
#define HEIGHT 1
extern int X;
extern int Y;

// colors used by the test program
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0xFF000000

// variables for xshm
extern Display *display;
extern Window rootWindow;
extern XShmSegmentInfo shminfo;
extern XImage *image;

extern uint64_t xshm_start_time;
extern uint64_t xshm_end_time;

void initXShm();
void closeXShm()
unsigned int getPixelColor();
unsigned int getPixelColorX();
void wait_for_color(unsigned int color);
void cleanup_xshm();
void measure_fw_latency(int input_fd);

#endif
