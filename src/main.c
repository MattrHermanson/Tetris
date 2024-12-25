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
  Rectangle parentRect;
  Rectangle childRect;
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
  Block testBlock = {{0, 0, 40, 40}, {0, 40, 120, 40}, BLUE};

  //40x40 blocks

  InitWindow(screenWidth, screenHeight, "Drawing a Square");

  SetTargetFPS(60);

  float tickTimer = 0;

  
  while (!WindowShouldClose()) {

    float deltaTime = GetFrameTime();
    tickTimer += deltaTime;
    
    BeginDrawing(); 

    ClearBackground(RAYWHITE);
    drawGameGrid(gameGrid, 15, 10);
    drawBlock(testBlock);

    if (tickTimer > 1 && testBlock.childRect.y < 560) {
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
  DrawRectangleRec(block.parentRect, block.color);
  DrawRectangleRec(block.childRect, block.color);
}

void moveBlock (Block* block, char axis, float dist) {
  switch (axis) {
    case 'x':
      block -> parentRect.x += dist;
      block -> childRect.x += dist;
      break;

    case 'y':
      block -> parentRect.y += dist;
      block -> childRect.y += dist;
      break;
  }
  
}
