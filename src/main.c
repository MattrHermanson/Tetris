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
  Rectangle tiles[4];
  int rotaionIndex;
  Color color;
} Block;

void createGameGrid(GridLine* grid, int rows, int cols, int size);
void drawGameGrid(GridLine* grid, int rows, int cols);
void drawBlock(Block block);
void moveBlock(Block* block, char axis, float dist);
void rotateBlock(Block* block, bool direction);
void addTiles(int tileBoard[screenWidth/gridBlockSize][screenHeight/gridBlockSize], Block* block);
void removeTiles(int tileBoard[screenWidth/gridBlockSize][screenHeight/gridBlockSize], Block* block);
int checkCollisionTiles(int tileBoard[screenWidth/gridBlockSize][screenHeight/gridBlockSize], Rectangle newRect);
bool isInBounds(int x, int y);
void createIBlock(Block *block, int x, int y, Color color);
void createJBlock(Block *block, int x, int y, Color color);
void createLBlock(Block *block, int x, int y, Color color);
void createOBlock(Block *block, int x, int y, Color color);
void createSBlock(Block *block, int x, int y, Color color);
void createTBlock(Block *block, int x, int y, Color color);
void createZBlock(Block *block, int x, int y, Color color);

//an array that keeps track if there is a tile in that grid spot
int board [screenWidth/gridBlockSize][screenHeight/gridBlockSize];

int main (void) {

  GridLine gameGrid[23];
  createGameGrid(gameGrid, 15, 10, 40);
  Block testBlock;
  createTBlock(&testBlock, 120, 80, RED);

  Block testBlock2;
  createJBlock(&testBlock2, 160, 240, BLUE);

  InitWindow(screenWidth, screenHeight, "Drawing a Square");

  SetTargetFPS(60);

  float tickTimer = 0;

  //TODO add in wall kick tests
  //fix tile collision not working
  
  while (!WindowShouldClose()) {

    float deltaTime = GetFrameTime();
    tickTimer += deltaTime;
    
    BeginDrawing(); 

    ClearBackground(RAYWHITE);
    drawGameGrid(gameGrid, 15, 10);
    drawBlock(testBlock);
    drawBlock(testBlock2);

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
  for (int i = 0; i < 4; i++) {
    DrawRectangleRec(block.tiles[i], block.color);
  } 
}

void moveBlock (Block* block, char axis, float dist) {
  switch (axis) {
    case 'x':
      //check if inbounds of screen
      for (int i = 0; i < 4; i++) {
        if (isInBounds((block -> tiles[i].x + dist), block -> tiles[i].y) == false) {
          return;
        }
      }

      removeTiles(board, block);

      //move block
      for (int i = 0; i < 4; i++) {
        block -> tiles[i].x += dist;
      }
      
      //check collision on all 4 tiles of the block
      for (int i = 0; i < 4; i++) {
        if (checkCollisionTiles(board, block -> tiles[i]) == 1) {

          //undo block move
          for (int j = 0; j < 4; j++) {
            block -> tiles[j].x -= dist;
          }
          break;
        }
      }

      addTiles(board, block);
      
      break;

    case 'y':
      //same as x
      for (int i = 0; i < 4; i++) {
        if (isInBounds(block -> tiles[i].x, (block -> tiles[i].y + dist)) == false) {
          return;
        }
      }

      removeTiles(board, block);

      //move block
      for (int i = 0; i < 4; i++) {
        block -> tiles[i].y += dist;
      }

      //check collision on all 4 tiles of block
      for (int i = 0; i < 4; i++) {
        if (checkCollisionTiles(board, block -> tiles[i]) == 1) {

          //undo block move
          for (int j = 0; j < 4; j++) {
            block -> tiles[j].y -= dist;
          }
          break;
        }
      }

      addTiles(board, block);

      break;
  }
}

//rotate the block
//True = clockwise; False = counter clockwise
void rotateBlock (Block* block, bool direction) {
 
  Rectangle rectArr[4];
  for (int i = 0; i < 4; i++) {
    rectArr[i] = block -> tiles[i];
  }

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

  for (int i = 0; i < 4; i++) {
    block -> tiles[i] = rectArr[i];
  }
}

void addTiles (int tileBoard[screenWidth/gridBlockSize][screenHeight/gridBlockSize], Block* block) {
  for (int i = 0; i < 4; i++) {
    int x = block -> tiles[i].x;
    int y = block -> tiles[i].y;

    tileBoard[x/gridBlockSize][y/gridBlockSize] = 1;
  }  
}

void removeTiles (int tileBoard[screenWidth/gridBlockSize][screenHeight/gridBlockSize], Block* block) {
  for (int i = 0; i < 4; i++) {
    int x = block -> tiles[i].x;
    int y = block -> tiles[i].y;

    tileBoard[x/gridBlockSize][y/gridBlockSize] = 0;
  } 
}

//return 1 if move will result in collision
int checkCollisionTiles (int tileBoard[screenWidth/gridBlockSize][screenHeight/gridBlockSize], Rectangle newRect) {
  int x = newRect.x;
  int y = newRect.y;

  if (tileBoard[x/gridBlockSize][y/gridBlockSize] == 1) {
    return 1;
  } else {
    return 0;
  }
}

void createIBlock (Block* block, int x, int y, Color color) {
  block -> tiles[0].x = x;
  block -> tiles[0].y = y;
  block -> tiles[0].width = gridBlockSize;
  block -> tiles[0].height = gridBlockSize;

  block -> tiles[1].x = (x - gridBlockSize);
  block -> tiles[1].y = y;
  block -> tiles[1].width = gridBlockSize;
  block -> tiles[1].height = gridBlockSize;

  block -> tiles[2].x = (x + gridBlockSize);
  block -> tiles[2].y = y;
  block -> tiles[2].width = gridBlockSize;
  block -> tiles[2].height = gridBlockSize;

  block -> tiles[3].x = (x + (2 * gridBlockSize));
  block -> tiles[3].y = y; 
  block -> tiles[3].width = gridBlockSize; 
  block -> tiles[3].height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;

  addTiles(board, block);
}

void createJBlock (Block* block, int x, int y, Color color) {
  block -> tiles[0].x = x;
  block -> tiles[0].y = y;
  block -> tiles[0].width = gridBlockSize;
  block -> tiles[0].height = gridBlockSize;

  block -> tiles[1].x = (x - gridBlockSize);
  block -> tiles[1].y = y;
  block -> tiles[1].width = gridBlockSize;
  block -> tiles[1].height = gridBlockSize;

  block -> tiles[2].x = (x - gridBlockSize);
  block -> tiles[2].y = (y - gridBlockSize);
  block -> tiles[2].width = gridBlockSize;
  block -> tiles[2].height = gridBlockSize;

  block -> tiles[3].x = (x + gridBlockSize);
  block -> tiles[3].y = y; 
  block -> tiles[3].width = gridBlockSize; 
  block -> tiles[3].height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;

  addTiles(board, block);
}

void createLBlock (Block* block, int x, int y, Color color) {
  block -> tiles[0].x = x;
  block -> tiles[0].y = y;
  block -> tiles[0].width = gridBlockSize;
  block -> tiles[0].height = gridBlockSize;

  block -> tiles[1].x = (x - gridBlockSize);
  block -> tiles[1].y = y;
  block -> tiles[1].width = gridBlockSize;
  block -> tiles[1].height = gridBlockSize;

  block -> tiles[2].x = (x + gridBlockSize);
  block -> tiles[2].y = y;
  block -> tiles[2].width = gridBlockSize;
  block -> tiles[2].height = gridBlockSize;

  block -> tiles[3].x = (x + gridBlockSize);
  block -> tiles[3].y = (y - gridBlockSize); 
  block -> tiles[3].width = gridBlockSize; 
  block -> tiles[3].height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;

  addTiles(board, block);
}

void createOBlock (Block* block, int x, int y, Color color) {
  block -> tiles[0].x = x;
  block -> tiles[0].y = y;
  block -> tiles[0].width = gridBlockSize;
  block -> tiles[0].height = gridBlockSize;

  block -> tiles[1].x = x;
  block -> tiles[1].y = (y - gridBlockSize);
  block -> tiles[1].width = gridBlockSize;
  block -> tiles[1].height = gridBlockSize;

  block -> tiles[2].x = (x + gridBlockSize);
  block -> tiles[2].y = y;
  block -> tiles[2].width = gridBlockSize;
  block -> tiles[2].height = gridBlockSize;

  block -> tiles[3].x = (x + gridBlockSize);
  block -> tiles[3].y = (y - gridBlockSize); 
  block -> tiles[3].width = gridBlockSize; 
  block -> tiles[3].height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;

  addTiles(board, block);
}

void createSBlock (Block* block, int x, int y, Color color) {
  block -> tiles[0].x = x;
  block -> tiles[0].y = y;
  block -> tiles[0].width = gridBlockSize;
  block -> tiles[0].height = gridBlockSize;

  block -> tiles[1].x = (x - gridBlockSize);
  block -> tiles[1].y = y;
  block -> tiles[1].width = gridBlockSize;
  block -> tiles[1].height = gridBlockSize;

  block -> tiles[2].x = x;
  block -> tiles[2].y = (y - gridBlockSize);
  block -> tiles[2].width = gridBlockSize;
  block -> tiles[2].height = gridBlockSize;

  block -> tiles[3].x = (x + gridBlockSize);
  block -> tiles[3].y = (y - gridBlockSize); 
  block -> tiles[3].width = gridBlockSize; 
  block -> tiles[3].height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;

  addTiles(board, block);
}

void createTBlock (Block* block, int x, int y, Color color) {
  block -> tiles[0].x = x;
  block -> tiles[0].y = y;
  block -> tiles[0].width = gridBlockSize;
  block -> tiles[0].height = gridBlockSize;

  block -> tiles[1].x = (x - gridBlockSize);
  block -> tiles[1].y = y;
  block -> tiles[1].width = gridBlockSize;
  block -> tiles[1].height = gridBlockSize;

  block -> tiles[2].x = (x + gridBlockSize);
  block -> tiles[2].y = y;
  block -> tiles[2].width = gridBlockSize;
  block -> tiles[2].height = gridBlockSize;

  block -> tiles[3].x = x;
  block -> tiles[3].y = (y - gridBlockSize); 
  block -> tiles[3].width = gridBlockSize; 
  block -> tiles[3].height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;

  addTiles(board, block);
}

void createZBlock (Block* block, int x, int y, Color color) {
  block -> tiles[0].x = x;
  block -> tiles[0].y = y;
  block -> tiles[0].width = gridBlockSize;
  block -> tiles[0].height = gridBlockSize;

  block -> tiles[1].x = (x - gridBlockSize);
  block -> tiles[1].y = (y - gridBlockSize);
  block -> tiles[1].width = gridBlockSize;
  block -> tiles[1].height = gridBlockSize;

  block -> tiles[2].x = x;
  block -> tiles[2].y = (y - gridBlockSize);
  block -> tiles[2].width = gridBlockSize;
  block -> tiles[2].height = gridBlockSize;

  block -> tiles[3].x = (x + gridBlockSize);
  block -> tiles[3].y = y; 
  block -> tiles[3].width = gridBlockSize; 
  block -> tiles[3].height = gridBlockSize;
  block -> rotaionIndex = 0;
  block -> color = color;

  addTiles(board, block);
}

bool isInBounds (int x, int y) {
  if (x >= 0 && x <= (screenWidth - gridBlockSize) && y > 0 && y < screenHeight) {
    return true;
  }

  return false;
}
