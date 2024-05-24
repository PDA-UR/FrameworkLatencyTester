#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pixman.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

#define WIDTH  1920
#define HEIGHT 1080

unsigned long black, white;

/*
void toggleFullscreen() {
    Atom wmState = XInternAtom(display, "_NET_WM_STATE", True);
    Atom wmFullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", True);

    Atom wm_state   = XInternAtom (display, "_NET_WM_STATE", True );
    Atom wm_fullscreen = XInternAtom (display, "_NET_WM_STATE_FULLSCREEN", True );

    XChangeProperty(display, window, wm_state, XA_ATOM, 32,
		    PropModeReplace, (unsigned char *)&wm_fullscreen, 1);
}
*/

int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen;
    pixman_image_t *image;
    uint32_t *buffer = malloc(WIDTH*HEIGHT*4);
    int should_exit = 0;

    // Initialize X connection
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
    screen = DefaultScreen(display);

    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);
    // Create window
    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, WIDTH, HEIGHT, 0, black, black);
    //window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, WIDTH, HEIGHT, 0, 0, 0);
    XSelectInput(display, window, ButtonPressMask | KeyPressMask | ExposureMask);
    XMapWindow(display, window);

    // Initialize buffer with black
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        buffer[i] = 0xFF000000;
    }

    // Create pixman image
    image = pixman_image_create_bits(PIXMAN_a8r8g8b8, WIDTH, HEIGHT, buffer, WIDTH * 4);
    //buffer = pixman_image_get_data(image);
    //if (buffer == NULL) {
    //    fprintf(stderr, "Failed to create buffer\n");
    //    exit(1);
    //}


    // Event loop
    while (!should_exit) {
        XNextEvent(display, &event);
        switch (event.type) {
            case ButtonPress:
                // Change window color to white upon mouse click
                for (int i = 0; i < WIDTH * HEIGHT; ++i) {
                    buffer[i] = 0xFFFFFFFF;
                }
    		image = pixman_image_create_bits(PIXMAN_a8r8g8b8, WIDTH, HEIGHT, buffer, WIDTH * 4);
                pixman_image_composite(PIXMAN_OP_SRC, image, NULL, image, 0, 0, 0, 0, 0, 0, WIDTH, HEIGHT);
                XClearWindow(display, window);
                XFlush(display);
                break;
            case KeyPress:
                should_exit = 1;
                break;
            case Expose:
                // Redraw the window
                XClearWindow(display, window);
                pixman_image_composite(PIXMAN_OP_SRC, image, NULL, image, 0, 0, 0, 0, 0, 0, WIDTH, HEIGHT);
                XFlush(display);
                break;
        }
    }

    // Cleanup
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    pixman_image_unref(image);
    return 0;
}
