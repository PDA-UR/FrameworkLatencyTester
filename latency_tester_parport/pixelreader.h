#ifndef READ_PIXEL_H
#define READ_PIXEL_H

#include <cstdint>

// position and dimension (should only one pixel!) of the region observed by XShm
#define WIDTH 1
#define HEIGHT 1

// colors used by the test program
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0xFF000000

class PixelReader {
    private:
        bool measure;
        //unsigned int getPixelColorX();

    protected:
        int X;
        int Y;
        bool running;

    public:
        uint64_t start_time;
        uint64_t end_time;
        uint64_t read_start_time;
        uint64_t read_end_time;

        PixelReader(int x, int y);
        unsigned int getPixelColor();
        void wait_for_color(unsigned int color);
        void measure_fw_latency();
        void trigger_measurement();
        void reset();
        void cleanup();
};

#endif
