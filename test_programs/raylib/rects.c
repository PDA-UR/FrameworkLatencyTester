#include "raylib.h"
#include <stdlib.h>

// screen size
#define WIDTH 1920
#define HEIGHT 1080

const int n_rects = 1000;
int rect_w = WIDTH / 10;
int rect_h = HEIGHT / 10;

typedef struct ColoredRect {
	Rectangle rect;
	Color color;
} ColoredRect;

void create_rects(ColoredRect *rects)
{
    for (int i = 0; i < n_rects; i++)
    {
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;

        int x = rect_w + rand() % (WIDTH - 2 * rect_w);
        int y = rand() % (HEIGHT - rect_h);

        Color color = {r, g, b, 0xff};
		Rectangle rec = { x, y, rect_w, rect_h };
		ColoredRect colRec = { rec, color };
		rects[i] = colRec;
	}
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "raylib default");

	ToggleFullscreen();

    Rectangle white_rect = { 0, 0, 300, HEIGHT };

	ColoredRect rects[n_rects];

	// let's not limit ourselves - maybe this is stupid
    //SetTargetFPS(60);
	
	int is_pressed = 0;

    while (!WindowShouldClose()) {

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			create_rects(rects);
			is_pressed = 1;
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			is_pressed = 0;
		}

        BeginDrawing();

			if (is_pressed) {
				for (int i = 0; i < n_rects; i++) {
					DrawRectangleRec(rects[i].rect, rects[i].color);
				}
				DrawRectangleRec(white_rect, WHITE);
			} else {
				ClearBackground(BLACK);
			}

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
