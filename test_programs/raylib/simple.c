#include "raylib.h"

int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "raylib default");

	ToggleFullscreen();

    Rectangle rec = { 0, 0, screenWidth, screenHeight };

	// let's not limit ourselves - maybe this is stupid
    //SetTargetFPS(60);
	
	int draw = 0;

    while (!WindowShouldClose()) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			draw = 1;
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			draw = 0;
		}

        BeginDrawing();

            ClearBackground(BLACK);

			if (draw) {
				DrawRectangleRec(rec, WHITE);
			}

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
