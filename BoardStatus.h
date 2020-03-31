#pragma once

#include "game.h"

class BoardStatus
{
public:
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
    int numboard[11][10] = {
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0, 10,  0,  0,  0,  0,  0, 11,  0 },
        {  0, 12,  0, 13,  0, 14,  0, 15,  0, 16 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0, 17,  0, 18,  0, 19,  0, 20,  0, 21 },
        {  0,  0, 22,  0,  0,  0,  0,  0, 23,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
    };                                                                      // remember that 5 and 28 are governer and general
    std::set<std::pair<int, int>> choosings;                                // store one or two "choosing" piece(s)

    int numquery(std::pair<int, int> pos);

    char chrquery(std::pair<int, int> pos);
};