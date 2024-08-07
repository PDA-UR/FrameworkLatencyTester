#ifndef XSHM_READER_H
#define XSHM_READER_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xext.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "pixelreader.h"

class XShmReader : public PixelReader
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
        XShmReader(int x, int y);// : PixelReader(int x, int y);
        unsigned int getPixelColor(); // : PixelReader::getPixelColor();
        void cleanup();
};

#endif
