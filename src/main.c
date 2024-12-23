#include "raylib.h"

#define gridWidth 5
#define gridColor GRAY

typedef struct GridLine {
  Vector2 pos;
  int width;
  Color color;
} GridLine;

void createGameGrid(GridLine* grid, int rows, int cols, int size);
void drawGameGrid(GridLine* grid, int rows, int cols);

int main (void) {

  const int screenWidth = 400;
  const int screenHeight = 600;

  GridLine gameGrid[23];
  createGameGrid(gameGrid, 15, 10, 40);

  //40x40 blocks

  InitWindow(screenWidth, screenHeight, "Drawing a Square");

  SetTargetFPS(60);


  while (!WindowShouldClose()) {
    
    BeginDrawing(); 

    ClearBackground(RAYWHITE);
    drawGameGrid(gameGrid, 15, 10);


    EndDrawing();
  }

  CloseWindow();
  return 0;
}

void createGameGrid (GridLine* grid, int rows, int cols, int size) {

  //create column grid lines
  for (int i = 0; i < (rows-1); i++) {
    int xOffset = i * size;
    GridLine newLine = {{xOffset, 0}, gridWidth, gridColor};
    grid[i] = newLine;
  }

  //create row grid lines
  for (int i = 0; i < (cols-1); i++) {
    int yOffset = i * size;
    GridLine newLine = {{0, yOffset}, gridWidth, gridColor};
    grid[i] = newLine;
  }
}

//TODO not drawing grid the right way
void drawGameGrid (GridLine* grid, int rows, int cols) {

  for (int i = 0; i < (rows-1); i++) {
    DrawRectangle(grid[i].pos.x, grid[i].pos.y, 400, grid[i].width, grid[i].color);
  }

  for (int i = 0; i < (cols-1); i++) {
    DrawRectangle(grid[i].pos.x, grid[i].pos.y, grid[i].width, 600, grid[i].color);
  }
}
