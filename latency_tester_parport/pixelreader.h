#ifndef READ_PIXEL_H
#define READ_PIXEL_H

#include <sys/shm.h>
#include <X11/extensions/XShm.h>

// position and dimension (should only one pixel!) of the region observed by XShm
#define WIDTH 1
#define HEIGHT 1

// colors used by the test program
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0xFF000000

class PixelReader {
    private:
        int X;
        int Y;
        bool measure;
        unsigned int getPixelColor();
        //unsigned int getPixelColorX();

    public:
        uint64_t start_time;
        uint64_t end_time;

        PixelReader(int x, int y);
        void wait_for_color(unsigned int color);
        void measure_fw_latency(int input_fd);
        void trigger_measurement;
        void reset();
        void cleanup();
}

#endif
