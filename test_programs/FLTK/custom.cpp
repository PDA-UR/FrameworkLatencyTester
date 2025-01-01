#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <cstdlib>

#define WIDTH 1920
#define HEIGHT 1080
#define X_OFFSET 300

int rect_count = 0;

class MainWindow : public Fl_Double_Window
{
    public:
        MainWindow() : Fl_Double_Window(0, 0, WIDTH, HEIGHT)
    {
        color(FL_BLACK);
        label("FLTK Test Application");
        fullscreen();
    }
        bool pressed = false;

    protected:
        void draw() override
        {
            if(pressed)
            {
                fl_color(color());
                fl_rectf(0, 0, w(), h());
                for (int i = 0; i < rect_count; i++)
                {
                    float r = rand() % 255;
                    float g = rand() % 255;
                    float b = rand() % 255;
                    float a = rand() % 255;
                    fl_color(r, g, b);

                    int x = X_OFFSET + rand() % (w() - (2 * X_OFFSET));
                    int y = rand() % (h() - X_OFFSET);
                    int width = rand() % (w() - x);
                    int height = rand() % (h() - y);

                    fl_rectf(x, y, width, height);
                }
                fl_color(FL_WHITE);
                fl_rectf(0, 0, X_OFFSET, HEIGHT);
            }
            else
            {
                fl_color(FL_BLACK);
                fl_rectf(0, 0, w(), h());
            }
        }

        int handle(int event) override
        {
            switch (event)
            {
                case FL_PUSH:
                    pressed = true;
                    redraw();
                    return 1;
                case FL_RELEASE:
                    pressed = false;
                    redraw();
                    return 1;
            }
            return Fl_Double_Window::handle(event);
        }
};

int main(int argc, char **argv)
{
	if (argc > 1)
	{
		rect_count = atoi(argv[1]) - 1;
	}

    MainWindow window;
    window.show();
    return Fl::run();
}
