#pragma once

#include "game.h"
#include "Piece.h"

void bettersolidcircle(int x, int y, int radius);

void drawboard(const int seam = 4, const int dest = 12);

void drawpiece(int x, int y, COLORREF color, const wchar_t* symbol);

void drawpiece(int xPos, int yPos, char symbolchar);

void drawpiece(Piece piece);