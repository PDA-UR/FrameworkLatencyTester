#include "main.h"
#include "vblank.h"

using namespace std;

VblankHandler::VblankHandler()
{
    vsync_count = 0;
    measure_vblank = 0;
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
}

void VblankHandler::get_vblanks()
{
	unsigned int last_sync_count = 0;
	uint64_t last_vblank_time = get_micros();

	while(measuring)
	{
		unsigned int sync_count;
		glXGetVideoSyncSGI(&sync_count);

		if (sync_count != last_sync_count)
		{
			uint64_t new_vblank_time = get_micros();
			last_sync_count = sync_count;

			if (measure_vblank == 1)
			{
				vsync_time[vsync_count] = new_vblank_time;			
				vsync_count++;
			}
		}

		usleep(10);
	}
}
