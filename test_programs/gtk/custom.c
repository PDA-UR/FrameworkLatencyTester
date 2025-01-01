#include <cairo.h>
#include <gtk/gtk.h>

static void draw(cairo_t *);

int pressed = 0;
float r;
float g;
float b;
float a;

int rect_count = 0;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    draw(cr);

    return 0;
}

static gboolean on_click_event(GtkWidget *widget, gpointer data)
{
    pressed = 1;
    gtk_widget_queue_draw(widget);
    return 0;
}

static gboolean on_release_event(GtkWidget *widget, gpointer data)
{
    pressed = 0;
    gtk_widget_queue_draw(widget);
    return 0;
}

static void draw(cairo_t *cr)
{
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, 0.0, 0.0);
    cairo_rectangle(cr, 0, 0, 1920, 1080);
    cairo_fill(cr);

    if(pressed)
    {
        for (int i = 0; i < rect_count; i++)
        {
            r = (float)rand() / RAND_MAX;
            g = (float)rand() / RAND_MAX;
            b = (float)rand() / RAND_MAX;
            a = 1.0f; //(float)rand() / RAND_MAX;
            cairo_set_source_rgba(cr, r, g, b, a);

            int x = 300 + rand() % (1920 - 300);
            int y = rand() % 1080;
            //int width = rand() % (1920 - x);
            //int height = rand() % (1080 - y);
            int width = 100 + rand() % 500;
            int height = 100 + rand() % 500;

            cairo_rectangle(cr, x, y, width, height);
            cairo_fill(cr);
        }
        cairo_set_source_rgba(cr, 1, 1, 1, 1);
        cairo_rectangle(cr, 0, 0, 300, 1080);
        cairo_fill(cr);
    }
}

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		rect_count = atoi(argv[1]) - 1;
	}

    GtkWidget *window;
    GtkWidget *drawing_area;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_fullscreen((GtkWindow*)window);

    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL); 
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "button-press-event", G_CALLBACK(on_click_event), NULL); 
    g_signal_connect(window, "button-release-event", G_CALLBACK(on_release_event), NULL); 

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080); 
    gtk_window_set_title(GTK_WINDOW(window), "GTK Test Application");

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
