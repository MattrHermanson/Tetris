#include "raylib.h"
#include "stdio.h"

#define gridWidth 2
#define gridColor GRAY

typedef struct GridLine {
  Vector2 pos;
  int width;
  Color color;
} GridLine;

typedef struct Block {
  Rectangle rect0;
  Rectangle rect1;
  Rectangle rect2;
  Rectangle rect3;
  Color color;
} Block;

void createGameGrid(GridLine* grid, int rows, int cols, int size);
void drawGameGrid(GridLine* grid, int rows, int cols);
void drawBlock(Block block);
void moveBlock(Block* block, char axis, float dist);

int main (void) {

  const int screenWidth = 400;
  const int screenHeight = 600;

  GridLine gameGrid[23];
  createGameGrid(gameGrid, 15, 10, 40);
  Block testBlock = {{80, 40, 40, 40}, {40, 40, 40, 40}, {80, 0, 40, 40}, {120, 0, 40, 40}, BLUE};

  //40x40 blocks

  InitWindow(screenWidth, screenHeight, "Drawing a Square");

  SetTargetFPS(60);

  float tickTimer = 0;

  //TODO figure out rotation matrix thing
  //standardize block size ie 40 across program with a variable
  //make create block functions for tetrominoes i,j,l,o,s,t,z
  //make bound checker
  //make tile checker
  
  while (!WindowShouldClose()) {

    float deltaTime = GetFrameTime();
    tickTimer += deltaTime;
    
    BeginDrawing(); 

    ClearBackground(RAYWHITE);
    drawGameGrid(gameGrid, 15, 10);
    drawBlock(testBlock);

    if (tickTimer > 1) {
      moveBlock(&testBlock, 'y', 40);
      tickTimer -= 1;
    }

    if (IsKeyPressed(KEY_A)) {
      moveBlock(&testBlock, 'x', -40);
    }
    if (IsKeyPressed(KEY_D)) {
      moveBlock(&testBlock, 'x', 40);
    }


    EndDrawing();
  }

  CloseWindow();
  return 0;
}

void createGameGrid (GridLine* grid, int rows, int cols, int size) {

  //create column grid lines
  for (int i = 0; i < rows; i++) {
    int yOffset = i * size;
    GridLine newLine = {{0, yOffset}, gridWidth, gridColor};
    grid[i] = newLine;
  }

  for (int j = 0; j < cols; j++) {
    int xOffset = j * size;
    GridLine newLine = {{xOffset, 0}, gridWidth, gridColor};
    grid[j + rows] = newLine;
  }

}

void drawGameGrid (GridLine* grid, int rows, int cols) {

  for (int i = 0; i < (rows); i++) {
    DrawRectangle(grid[i].pos.x, grid[i].pos.y, 400, grid[i].width, grid[i].color);
  }
 
  int endBound = rows + cols;
  for (int i = rows; i <  endBound; i++) {
    DrawRectangle(grid[i].pos.x, grid[i].pos.y, grid[i].width, 600, grid[i].color);
  }

}

void drawBlock (Block block) {
  DrawRectangleRec(block.rect0, block.color);
  DrawRectangleRec(block.rect1, block.color);
  DrawRectangleRec(block.rect2, block.color);
  DrawRectangleRec(block.rect3, block.color);
}

void moveBlock (Block* block, char axis, float dist) {
  switch (axis) {
    case 'x':
      block -> rect0.x += dist;
      block -> rect1.x += dist;
      block -> rect2.x += dist;
      block -> rect3.x += dist;
      break;

    case 'y':
      block -> rect0.y += dist;
      block -> rect1.y += dist;
      block -> rect2.y += dist;
      block -> rect3.y += dist;
      break;
  }
  
}
