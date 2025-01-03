#include "xshm_reader.h"
#include "pixelreader.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xext.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

using namespace std;


XShmReader::XShmReader(int x, int y) : PixelReader(x, y)
{
    initXShm();
}

// initialize the XShm extension to be able to read one pixel from the screen
void XShmReader::initXShm()
{
    //cout << "start init XShm" << endl;
    XWindowAttributes windowAttributes;
    Screen *screen;

    //cout << "get display..." << endl;
    display = XOpenDisplay(getenv("DISPLAY"));
    //cout << "get root..." << endl;
    rootWindow = DefaultRootWindow(display);
    //cout << "get window attributes..." << endl;
    XGetWindowAttributes(display, rootWindow, &windowAttributes);
    screen = windowAttributes.screen;

    // create an image object that will store the pixel data
    //cout << "create image..." << endl;
    image = XShmCreateImage(display, DefaultVisualOfScreen(screen), DefaultDepthOfScreen(screen), ZPixmap, NULL, &shminfo, WIDTH, HEIGHT);

    //cout << "get shmid..." << endl;
    shminfo.shmid = shmget(IPC_PRIVATE, image->bytes_per_line * image->height, IPC_CREAT|0777);
    //cout << "get shmaddr..." << endl;
    shminfo.shmaddr = image->data = (char*)shmat(shminfo.shmid, 0, 0);
    shminfo.readOnly = False;
    XShmAttach(display, &shminfo);
    //cout << "end init xshm" << endl;
}

// detach XShm and clean up memory
// this has to be called when the program terminates
void XShmReader::closeXShm()
{
    XShmDetach(display, &shminfo);
    XDestroyImage(image);
}

// get pixel at specified position with XShm
unsigned int XShmReader::getPixelColor()
{
    //PixelReader::getPixelColor();

    //cout << "before xshmgetimage" << endl;

    //auto result = XShmGetImage(display, rootWindow, image, X, Y, 0x00ffffff);
    auto result = XShmGetImage(display, rootWindow, image, 200, 200, 0x00ffffff);

    //cout << "getPixelData " << image->data[2] << endl;

    return image->data[2]; // red channel is enough for us
}

void XShmReader::cleanup()// : PixelReader::cleanup()
{
    cout << "cleanup xshm" << endl;
    closeXShm();
}
/*
unsigned int PixelReader::getPixelColorX()
{
    XColor c;
    XImage *image;

    image = XGetImage (display, rootWindow, X, Y, 1, 1, AllPlanes, XYPixmap);

    c.pixel = XGetPixel (image, 0, 0);

    XFree (image);

    XQueryColor (display, XDefaultColormap(display, XDefaultScreen (display)), &c);

    return c.red / 256;
}
*/
