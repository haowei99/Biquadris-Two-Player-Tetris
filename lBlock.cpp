#include "lBlock.h"

lBlock::lBlock(int x, int y, int level, Board *board): Block{x, y, level, board} {
    currShape[1][2] = true;
    currShape[2][0] = true;
    currShape[2][1] = true;
    currShape[2][2] = true;
}

void lBlock::rotateClockwise() {
    bool rotations[4][3][3] = {
        {{0,0,0},
         {0,0,1},
         {1,1,1}},

        {{1,0,0},
         {1,0,0},
         {1,1,0}},

        {{0,0,0},
         {1,1,1},
         {1,0,0}},

        {{1,1,0},
         {0,1,0},
         {0,1,0}}
    };
}

void lBlock::rotateCounterClockwise() {
    bool rotations[4][3][3] = {
        {{0,0,0},
         {0,0,1},
         {1,1,1}},
        
        {{1,1,0},
         {0,1,0},
         {0,1,0}},

        {{0,0,0},
         {1,1,1},
         {1,0,0}},

        {{1,0,0},
         {1,0,0},
         {1,1,0}}
        
    };
}

void lBlock::right() {
    x = x + 1;
}


void lBlock::left() {
    x = x - 1;
}

void lBlock::down() {
    y = y + 1;
}