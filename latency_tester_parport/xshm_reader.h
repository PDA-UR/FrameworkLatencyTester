#ifndef XSHM_READER_H
#define XSHM_READER_H

#include "pixelreader.h"

class XShmReader : PixelReader
{
    private:
        // variables for xshm
        Display *display;
        Window rootWindow;
        XShmSegmentInfo shminfo;
        XImage *image;

        void initXShm();
        void closeXShm();

    public:
        XShmReader(int x, int y) : PixelReader(int x, int y);
}

#endif
