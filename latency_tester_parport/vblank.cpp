#include "main.h"
#include "vblank.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>

using namespace std;

VblankHandler::VblankHandler()
{
    vsync_count = 0;
    measure_vblank = 0;

    running = true;
    measure = false;

    measure_vblank_thread = thread(&VblankHandler::get_vblanks, this);
    //measure_vblank_thread.run();
}

void VblankHandler::get_vblanks()
{
	cout << "# vblank 1" << endl;
	//cout << "GLX_BAD_CONTEXT " << GLX_BAD_CONTEXT << endl; 

	// TODO should be in constructor?
	// but then it returns GLX_BAD_CONTEXT
	// maybe something with threads
	Display *glxDisplay = XOpenDisplay(NULL);
	if (!glxDisplay) {
		cout << "Error: Unable to open X display." << endl;
		return;
	}

	int default_screen_id = DefaultScreen(glxDisplay);
	Window glxRoot = RootWindow(glxDisplay, default_screen_id);

	// Create GLX context
	static int visual_attribs[] = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,
		None
	};
	XVisualInfo *visual = glXChooseVisual(glxDisplay, default_screen_id, visual_attribs);
	GLXContext context = glXCreateContext(glxDisplay, visual, NULL, GL_TRUE);
	glXMakeCurrent(glxDisplay, glxRoot, context);

	// Load GLX_SGI_video_sync extension
	glXGetVideoSyncSGI = 
		(GLXGETVIDEOSYNCSGIPROC)glXGetProcAddressARB((const GLubyte *)"glXGetVideoSyncSGI");
	if (!glXGetVideoSyncSGI) {
		cout << "Error: GLX_SGI_video_sync extension not supported.\n" << endl;
		return;
	}

	unsigned int last_sync_count = 0;
	uint64_t last_vblank_time = get_micros();

	while(running)
	{
		unsigned int sync_count;
		int ret = glXGetVideoSyncSGI(&sync_count);
		//cout << "sync count: " << sync_count << "; " << ret << endl;

		if (sync_count != last_sync_count)
		{
			uint64_t new_vblank_time = get_micros();
			last_sync_count = sync_count;

			if (measure == 1)
			{
				vsync_time[vsync_count] = new_vblank_time;			
				vsync_count++;
			}
		}

		usleep(10);
	}
}

void VblankHandler::cleanup()
{
    running = false;
    measure_vblank_thread.join();
}
