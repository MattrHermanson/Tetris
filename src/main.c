#include "raylib.h"
#include <stdio.h>

#define gridWidth 2
#define gridBlockSize 40
#define screenWidth 400
#define screenHeight 600
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
  int rotaionIndex;
  Color color;
} Block;

void createGameGrid(GridLine* grid, int rows, int cols, int size);
void drawGameGrid(GridLine* grid, int rows, int cols);
void drawBlock(Block block);
void moveBlock(Block* block, char axis, float dist);
void rotateBlock(Block* block, bool direction);
bool isInBounds(int x, int y);

//an array that keeps track if there is a tile in that grid spot
int board [screenWidth/gridBlockSize][screenHeight/gridBlockSize];

int main (void) {

  GridLine gameGrid[23];
  createGameGrid(gameGrid, 15, 10, 40);
  Block testBlock = {{80, 40, gridBlockSize, gridBlockSize}, {40, 40, gridBlockSize, gridBlockSize}, {80, 0, gridBlockSize, gridBlockSize}, {120, 0, gridBlockSize, gridBlockSize}, 0, BLUE};

  InitWindow(screenWidth, screenHeight, "Drawing a Square");

  SetTargetFPS(60);

  float tickTimer = 0;

  //TODO add in wall kick tests
  //make implement tile checking and tile updating in move and creation functions
  
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
    if (IsKeyPressed(KEY_X)) {
      rotateBlock(&testBlock, true);
    }
    if (IsKeyPressed(KEY_Z)) {
      rotateBlock(&testBlock, false);
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
      if (isInBounds((block -> rect0.x + dist), block -> rect0.y) == false ||
          isInBounds((block -> rect1.x + dist), block -> rect1.y) == false ||
          isInBounds((block -> rect2.x + dist), block -> rect2.y) == false ||
          isInBounds((block -> rect3.x + dist), block -> rect3.y == false)) {
        return; 
      }

      block -> rect0.x += dist;
      block -> rect1.x += dist;
      block -> rect2.x += dist;
      block -> rect3.x += dist;
      break;

    case 'y':
      if (isInBounds(block -> rect0.x, (block -> rect0.y + dist)) == false ||
          isInBounds(block -> rect1.x, (block -> rect1.y + dist)) == false ||
          isInBounds(block -> rect2.x, (block -> rect2.y + dist)) == false ||
          isInBounds(block -> rect3.x, (block -> rect3.y + dist))  == false) {
        return; 
      }

      block -> rect0.y += dist;
      block -> rect1.y += dist;
      block -> rect2.y += dist;
      block -> rect3.y += dist;
      break;
  }
  
}

//rotate the block
//True = clockwise; False = counter clockwise
void rotateBlock (Block* block, bool direction) {
 
  Rectangle rectArr[] = {block -> rect0, block -> rect1, block -> rect2, block -> rect3};

  for (int i = 1; i < 4; i++) {
    rectArr[i].x -= rectArr[0].x;
    rectArr[i].y -= rectArr[0].y;
  }

  if (direction == true) {
    //Clockwise
    for (int i = 1; i < 4; i++) {
      //[0 1][-1 0] clockwise rotate matrix
      int newX = (rectArr[i].x * 0) + (rectArr[i].y * 1);
      int newY = (rectArr[i].x * -1) + (rectArr[i].y * 0);
      rectArr[i].x = newX;
      rectArr[i].y = newY;
    } 
  } else {
    //Counter-Clockwise
    for (int i = 1; i < 4; i++) {
      //[0 -1][1 0] counter clock rotate matrix
      int newX = (rectArr[i].x * 0) + (rectArr[i].y * -1);
      int newY = (rectArr[i].x * 1) + (rectArr[i].y * 0);
      rectArr[i].x = newX;
      rectArr[i].y = newY;
    }
  }

  for (int i = 1; i < 4; i++) {
    rectArr[i].x += rectArr[0].x;
    rectArr[i].y += rectArr[0].y;
  }

  block -> rect1 = rectArr[1];
  block -> rect2 = rectArr[2];
  block -> rect3 = rectArr[3];
}

void createIBlock (Block* block, int x, int y, Color color) {
  block -> rect0.x = x;
  block -> rect0.y = y;
  block -> rect0.width = gridBlockSize;
  block -> rect0.height = gridBlockSize;

  block -> rect1.x = (x - gridBlockSize);
  block -> rect1.y = y;
  block -> rect1.width = gridBlockSize;
  block -> rect1.height = gridBlockSize;

  block -> rect2.x = (x + gridBlockSize);
  block -> rect2.y = y;
  block -> rect2.width = gridBlockSize;
  block -> rect2.height = gridBlockSize;

  block -> rect3.x = (x + (2 * gridBlockSize));
  block -> rect3.y = y; 
  block -> rect3.width = gridBlockSize; 
  block -> rect3.height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;
}

void createJBlock (Block* block, int x, int y, Color color) {
  block -> rect0.x = x;
  block -> rect0.y = y;
  block -> rect0.width = gridBlockSize;
  block -> rect0.height = gridBlockSize;

  block -> rect1.x = (x - gridBlockSize);
  block -> rect1.y = y;
  block -> rect1.width = gridBlockSize;
  block -> rect1.height = gridBlockSize;

  block -> rect2.x = (x - gridBlockSize);
  block -> rect2.y = (y - gridBlockSize);
  block -> rect2.width = gridBlockSize;
  block -> rect2.height = gridBlockSize;

  block -> rect3.x = (x + (2 * gridBlockSize));
  block -> rect3.y = y; 
  block -> rect3.width = gridBlockSize; 
  block -> rect3.height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;
}

void createLBlock (Block* block, int x, int y, Color color) {
  block -> rect0.x = x;
  block -> rect0.y = y;
  block -> rect0.width = gridBlockSize;
  block -> rect0.height = gridBlockSize;

  block -> rect1.x = (x - gridBlockSize);
  block -> rect1.y = y;
  block -> rect1.width = gridBlockSize;
  block -> rect1.height = gridBlockSize;

  block -> rect2.x = (x + gridBlockSize);
  block -> rect2.y = y;
  block -> rect2.width = gridBlockSize;
  block -> rect2.height = gridBlockSize;

  block -> rect3.x = (x + gridBlockSize);
  block -> rect3.y = (y - gridBlockSize); 
  block -> rect3.width = gridBlockSize; 
  block -> rect3.height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;
}

void createOBlock (Block* block, int x, int y, Color color) {
  block -> rect0.x = x;
  block -> rect0.y = y;
  block -> rect0.width = gridBlockSize;
  block -> rect0.height = gridBlockSize;

  block -> rect1.x = x;
  block -> rect1.y = (y - gridBlockSize);
  block -> rect1.width = gridBlockSize;
  block -> rect1.height = gridBlockSize;

  block -> rect2.x = (x + gridBlockSize);
  block -> rect2.y = y;
  block -> rect2.width = gridBlockSize;
  block -> rect2.height = gridBlockSize;

  block -> rect3.x = (x + gridBlockSize);
  block -> rect3.y = (y - gridBlockSize); 
  block -> rect3.width = gridBlockSize; 
  block -> rect3.height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;
}

void createSBlock (Block* block, int x, int y, Color color) {
  block -> rect0.x = x;
  block -> rect0.y = y;
  block -> rect0.width = gridBlockSize;
  block -> rect0.height = gridBlockSize;

  block -> rect1.x = (x - gridBlockSize);
  block -> rect1.y = y;
  block -> rect1.width = gridBlockSize;
  block -> rect1.height = gridBlockSize;

  block -> rect2.x = x;
  block -> rect2.y = (y - gridBlockSize);
  block -> rect2.width = gridBlockSize;
  block -> rect2.height = gridBlockSize;

  block -> rect3.x = (x + gridBlockSize);
  block -> rect3.y = (y - gridBlockSize); 
  block -> rect3.width = gridBlockSize; 
  block -> rect3.height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;
}

void createTBlock (Block* block, int x, int y, Color color) {
  block -> rect0.x = x;
  block -> rect0.y = y;
  block -> rect0.width = gridBlockSize;
  block -> rect0.height = gridBlockSize;

  block -> rect1.x = (x - gridBlockSize);
  block -> rect1.y = y;
  block -> rect1.width = gridBlockSize;
  block -> rect1.height = gridBlockSize;

  block -> rect2.x = (x + gridBlockSize);
  block -> rect2.y = y;
  block -> rect2.width = gridBlockSize;
  block -> rect2.height = gridBlockSize;

  block -> rect3.x = x;
  block -> rect3.y = (y - gridBlockSize); 
  block -> rect3.width = gridBlockSize; 
  block -> rect3.height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;
}

void createZBlock (Block* block, int x, int y, Color color) {
  block -> rect0.x = x;
  block -> rect0.y = y;
  block -> rect0.width = gridBlockSize;
  block -> rect0.height = gridBlockSize;

  block -> rect1.x = (x - gridBlockSize);
  block -> rect1.y = (y - gridBlockSize);
  block -> rect1.width = gridBlockSize;
  block -> rect1.height = gridBlockSize;

  block -> rect2.x = x;
  block -> rect2.y = (y - gridBlockSize);
  block -> rect2.width = gridBlockSize;
  block -> rect2.height = gridBlockSize;

  block -> rect3.x = (x + gridBlockSize);
  block -> rect3.y = y; 
  block -> rect3.width = gridBlockSize; 
  block -> rect3.height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;
}

bool isInBounds(int x, int y) {
  if (x >= 0 && x < (screenWidth - gridBlockSize) && y > 0 && y < screenHeight) {
    return true;
  }

  return false;
}
