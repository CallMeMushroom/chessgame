#pragma once

#include "game.h"

class BoardStatus
{
public:
    bool turnflag = REDTURN;

    std::string chrboard[12] = {
    "..........",
    ".rhbagabhr",
    ".         ",
    ". c     c ",
    ".p p p p p",
    ".         ",
    ".         ",
    ".P P P P P",
    ". C     C ",
    ".         ",
    ".RHBAGABHR"
    };

    int numboard[11][10];

    int kingid[2] = { 0, 0 };

    std::set<std::pair<int, int>> choosings;                                // store one or two "choosing" piece(s)

    int getid(std::pair<int, int> pos);

    char getsymb(std::pair<int, int> pos);

    void modify(std::pair<int, int> pos, char symbol, int id);

    void modify(std::pair<int, int> pos, int id);

    bool importfrom(std::string str);
};