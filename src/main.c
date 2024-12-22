#include "raylib.h"

int main (void) {

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Drawing a Square");

  SetTargetFPS(60);

  int rectX = 400;
  int rectY = 225;

  while (!WindowShouldClose()) {
    
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("Here is a square", 190, 200, 20, BLACK);

    if (IsKeyDown(KEY_A)) {
      rectX -= 10;
    }

    if (IsKeyDown(KEY_D)) {
      rectX += 10;
    }

    DrawRectangle(rectX, rectY, 40, 40, BLACK);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
