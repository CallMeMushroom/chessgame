#pragma once

#include "game.h"
#include "util.h"
#include "BoardStatus.h"

#ifndef Piece_h
#define Piece_h

class Piece
{
public:
    int xPos, yPos;
    char name;
    int id;
    bool region;
    std::set<std::pair<int, int>> legalmoves;

    std::pair<int, int> getpos();

    void setpos(int xPos, int yPos);

    void setpos(std::pair<int, int> pos);

    void clear();

    bool inreach(BoardStatus board, int xPos, int yPos); 

    bool inreach(BoardStatus board, std::pair<int, int> pos);

    void maintain(BoardStatus board);

    Piece();

    Piece(BoardStatus board, std::pair<int, int> intersection);
};

#endif