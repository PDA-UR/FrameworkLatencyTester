#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <cstdlib>
#include <signal.h>

#define WIDTH 1920
#define HEIGHT 1080

bool running = true;

bool is_pressed = false;
bool do_redraw = true;

int n_horizontal = 10;
int n_vertical = 10;
int rect_w = WIDTH / n_horizontal;
int rect_h = HEIGHT / n_vertical;

int rect_count = 0;

// make sure we clean up when program is interrupted
void signalHandler(int sig)
{
    running = false;
    SDL_Quit();
    exit(sig);
}

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		rect_count = atoi(argv[1]) - 1;
	}

    signal(SIGINT, signalHandler);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(__FILE__, 0, 0, WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewInit();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    SDL_Event event;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble)WIDTH, (GLdouble)HEIGHT, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    //glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);

    while (running)
    {
        bool do_redraw = false;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    glClear(GL_COLOR_BUFFER_BIT);
                    for (int i = 0; i < rect_count; i++)
                    {
                        float r = (float)rand() / RAND_MAX;
                        float g = (float)rand() / RAND_MAX;
                        float b = (float)rand() / RAND_MAX;
                        glColor3f(r, g, b);

                        int x = 300 + rand() % (WIDTH - 300);
                        int y = rand() % HEIGHT;
                        int width = rand() % (WIDTH - x);
                        int height = rand() % (HEIGHT - y);

                        glBegin(GL_QUADS);
                        glVertex2i(x, y);
                        glVertex2i(x + width, y);
                        glVertex2i(x + width, y + height);
                        glVertex2i(x, y + height);
                        glEnd();
                    }

                    glColor3f(1, 1, 1);
                    glBegin(GL_QUADS);
                    glVertex2i(0, 0);
                    glVertex2i(300, 0);
                    glVertex2i(300, 1080);
                    glVertex2i(0, 1080);
                    glEnd();
                    glFlush();
                    SDL_GL_SwapWindow(window);
                    glClear(GL_COLOR_BUFFER_BIT);
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    glClearColor(0.0f, 0.0f, 0.0, 1.0f);
                    SDL_GL_SwapWindow(window);
                    glClear(GL_COLOR_BUFFER_BIT);
                }
            }
        }

        //SDL_GL_SwapWindow(window);
    }

    return 0;
}
