#include "raylib.h"

int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;
	
    InitWindow(screenWidth, screenHeight, "raylib image");

	ToggleFullscreen();

	Texture2D image = LoadTexture("noise.png");

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
				DrawTexture(image, 0, 0, WHITE);
			}

        EndDrawing();
    }

	UnloadTexture(image);

    CloseWindow();

    return 0;
}
