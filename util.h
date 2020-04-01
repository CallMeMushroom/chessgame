#pragma once

#include "game.h"
#include "util.h"

int distsquare(int x, int y, int x0, int y0);

int distsquare(std::pair<int, int> A, std::pair<int, int> B);

bool isfriend(char A, char B);

bool ischoosing(MOUSEMSG mouse, int xPos, int yPos);;