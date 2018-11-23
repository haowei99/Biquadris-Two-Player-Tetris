#ifndef __STARBLOCK_H__
#define __STARBLOCK_H__
#include "block.h"

class starBlock: public Block{
    bool currShape[1][1];
public:
    starBlock(int x, int y, int level, Board *board);
    void rotateClockwise() override;
    void rotateCounterClockwise() override;
    void right() override;
    void left() override;
    void down() override;
    void drop() override;
    bool canMoveDown() override;
    ~starBlock() = default;
};


#endif //__STARBLOCK_H__