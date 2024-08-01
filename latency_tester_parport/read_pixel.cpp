#include "main.h"
#include "read_pixel.h"

using namespace std;

PixelReader::PixelReader(int x, int y)
{
    X = x;
    Y = y;

    initXShm();
}

// initialize the XShm extension to be able to read one pixel from the screen
void PixelReader::initXShm()
{
    XWindowAttributes windowAttributes;
    Screen *screen;

    display = XOpenDisplay(getenv("DISPLAY"));
    rootWindow = DefaultRootWindow(display);
    XGetWindowAttributes(display, rootWindow, &windowAttributes);
    screen = windowAttributes.screen;

    // create an image object that will store the pixel data
    image = XShmCreateImage(display, DefaultVisualOfScreen(screen), DefaultDepthOfScreen(screen), ZPixmap, NULL, &shminfo, WIDTH, HEIGHT);

    shminfo.shmid = shmget(IPC_PRIVATE, image->bytes_per_line * image->height, IPC_CREAT|0777);
    shminfo.shmaddr = image->data = (char*)shmat(shminfo.shmid, 0, 0);
    shminfo.readOnly = False;
    XShmAttach(display, &shminfo);
}

// detach XShm and clean up memory
// this has to be called when the program terminates
void PixelReader::closeXShm()
{
    XShmDetach(display, &shminfo);
    XDestroyImage(image);
}

// get pixel at specified position with XShm
unsigned int PixelReader::getPixelColor()
{
    auto result = XShmGetImage(display, rootWindow, image, X, Y, 0x00ffffff);

    return image->data[2]; // red channel is enough for us
}

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

// wait until our pixel has a specified color
void PixelReader::wait_for_color(unsigned int color)
{
	unsigned int pixelColor;
	uint64_t start, end;

	while(1)
	{
		start = get_micros();
		pixelColor = getPixelColor();
		end = get_micros();

		if (pixelColor != 0)
		{
			xshm_start_time = start;
			xshm_end_time = end;
			return;
		}

		usleep(1);
	}
	return;
}

void PixelReader::measure_fw_latency(int input_fd)
{
    struct input_event inputEvent;
    int err = -1;

    while(measuring)
    {
        // read input events from the specified device
	// take 3 - 4 microseconds
        err = read(input_fd, &inputEvent, sizeof(struct input_event));

        // detected a left mouse click
        if( err > -1 &&
            inputEvent.type == EV_KEY &&
            inputEvent.code == MOUSE_BUTTON_LEFT &&
            inputEvent.value == CLICKED)
        {
		//cout << "evdev" << endl;
            start_time = get_micros();

            wait_for_color(COLOR_WHITE); // wait for test program to react

		//cout << "white" << endl;
            end_time = get_micros();
        }
    }
}

void PixelReader::cleanup()
{
    closeXShm();

}
