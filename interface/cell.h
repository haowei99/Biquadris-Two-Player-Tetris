#ifndef __CELL_H__
#define __CELL_H__
#include <iostream>
#include "window.h"

class Cell {
    bool isFilled, isBlind;
    char fillType;
    int x, y, width, height;
    int xcoord, ycoord;
    Xwindow * win;
    int colour;
    bool drop = false;

public:
    Cell();
    ~Cell();
    void setFill();
    void setCoords(int x, int y, int w, int h, Xwindow* window);
    void undraw();
    void draw();
    bool cellFilled();
    void setCell(char blockType);
    void unsetCell();
    int get_X();
    int get_Y();
    void set_X(int xCoor);
    void set_Y(int yCoor);
    void blind();
    void unblind();

    friend std::ostream& operator<<(std::ostream& os, Cell& cell);
};

std::ostream& operator<<(std::ostream& os, Cell& cell);

#endif // __CELL_H__
