import org.lwjgl.*;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import org.lwjgl.system.*;

import java.nio.*;

import static org.lwjgl.glfw.Callbacks.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;

import java.util.Random;

public class lwjgl3_custom {
	final int WIDTH = 1920;
	final int HEIGHT = 1080;
	final int RECT_WIDTH = 300;
	final int RECT_HEIGHT = 200;
	public int rect_count = 0;
	boolean clicked = false;
	boolean do_redraw = true;

	// The window handle
	private long window;
	Random rand;

	public void run() {
		//System.out.println("Hello LWJGL " + Version.getVersion() + "!");

		init();
		loop();

		// Free the window callbacks and destroy the window
		glfwFreeCallbacks(window);
		glfwDestroyWindow(window);

		// Terminate GLFW and free the error callback
		glfwTerminate();
		glfwSetErrorCallback(null).free();
	}

	void onMouseClick()
	{
		clicked = true;	
		do_redraw = true;	
	}

	void onMouseRelease()
	{
		clicked = false;	
		do_redraw = true;	
	}

	private void init() {
		rand = new Random();
		// Setup an error callback. The default implementation
		// will print the error message in System.err.
		GLFWErrorCallback.createPrint(System.err).set();

		// Initialize GLFW. Most GLFW functions will not work before doing this.
		if ( !glfwInit() )
			throw new IllegalStateException("Unable to initialize GLFW");

		// Configure GLFW
		glfwDefaultWindowHints(); // optional, the current window hints are already the default
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // the window will stay hidden after creation
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // the window will be resizable
		//glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE); // use system refresh rate
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

		// Create the window
		//window = glfwCreateWindow(WIDTH, HEIGHT, "lwjgl3_custom", glfwGetPrimaryMonitor(), NULL);

		// borderless fullscreen
		window = glfwCreateWindow(WIDTH, HEIGHT, "lwjgl3_custom", NULL, NULL);
		glfwSetWindowPos(window, WIDTH, 0);

		if ( window == NULL )
			throw new RuntimeException("Failed to create the GLFW window");

		//window.glfwSetWindowMonitor();

		// Setup a key callback. It will be called every time a key is pressed, repeated or released.
		glfwSetKeyCallback(window, (window, key, scancode, action, mods) -> {
			if ( key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE )
				glfwSetWindowShouldClose(window, true); // We will detect this in the rendering loop
		});

		glfwSetMouseButtonCallback(window, new GLFWMouseButtonCallback() {
			@Override
			public void invoke(long window, int button, int action, int mods) {
				if (action == GLFW_PRESS) {
					onMouseClick();
				} else {
					onMouseRelease();
				}
			}
		});

		// Make the OpenGL context current
		glfwMakeContextCurrent(window);
		// Enable v-sync
		//glfwSwapInterval(1);

		// Make the window visible
		glfwShowWindow(window);
	}

	private void loop() {
		// This line is critical for LWJGL's interoperation with GLFW's
		// OpenGL context, or any context that is managed externally.
		// LWJGL detects the context that is current in the current thread,
		// creates the GLCapabilities instance and makes the OpenGL
		// bindings available for use.
		GL.createCapabilities();

		// Set the clear color
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, (double)WIDTH, (double)HEIGHT, 0.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);

		// Run the rendering loop until the user has attempted to close
		// the window or has pressed the ESCAPE key.
		while ( !glfwWindowShouldClose(window) ) {
			if (do_redraw)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the framebuffer
				if (clicked) {
					for (int i = 0; i < rect_count; i++)
					{
						float r = rand.nextFloat();
						float g = rand.nextFloat();
						float b = rand.nextFloat();
						glColor3f(r, g, b);

						int x = rand.nextInt(RECT_WIDTH, WIDTH - RECT_WIDTH);
						int y = rand.nextInt(0, HEIGHT - RECT_HEIGHT);

						glBegin(GL_QUADS);
						glVertex2i(x, y);
						glVertex2i(x + RECT_WIDTH, y);
						glVertex2i(x + RECT_WIDTH, y + RECT_HEIGHT);
						glVertex2i(x, y + RECT_HEIGHT);
						glEnd();
					}

					glColor3f(1, 1, 1);
					glBegin(GL_QUADS);
					glVertex2i(0, 0);
					glVertex2i(RECT_WIDTH, 0);
					glVertex2i(RECT_WIDTH, HEIGHT);
					glVertex2i(0, HEIGHT);
					glEnd();
					glFlush();
				}
				glfwSwapBuffers(window); // swap the color buffers
				do_redraw = false;
			}

			// Poll for window events. The key callback above will only be
			// invoked during this call.
			glfwPollEvents();
		}
	}

	public static void main(String[] args) {
		int rect_count = 0;

		if (args.length > 0)
		{
			rect_count = Integer.parseInt(args[0]) - 1;
		}
		lwjgl3_custom application = new lwjgl3_custom();
		application.rect_count = rect_count;
		application.run();
	}

}
