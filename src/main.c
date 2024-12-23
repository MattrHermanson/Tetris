#include "raylib.h"

int main (void) {

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Drawing a Square");

  SetTargetFPS(60);

  Rectangle floor = {200, 300, 300, 10};
  Rectangle player = {400, 225, 40, 40};


  while (!WindowShouldClose()) {
    
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("Here is a square", 190, 200, 20, BLACK);

    if (IsKeyDown(KEY_A)) {
      player.x -= 10;
    }

    if (IsKeyDown(KEY_D)) {
      player.x += 10;
    }

    if (IsKeyPressed(KEY_SPACE)) {
      player.y -= 50;
    }

    if (CheckCollisionRecs(player, floor) == false) {
      player.y += 1;
    }

    DrawRectangle(floor.x, floor.y, floor.width, floor.height, GRAY);
    DrawRectangle(player.x, player.y, 40, 40, BLACK);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
