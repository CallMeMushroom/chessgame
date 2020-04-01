#include "BoardStatus.h"

int BoardStatus::numquery(std::pair<int, int> pos)
{
    return this->numboard[pos.first][pos.second];
}

char BoardStatus::chrquery(std::pair<int, int> pos)
{
    return this->chrboard[pos.first][pos.second];
}

void BoardStatus::modify(std::pair<int, int> pos, char symbol, int id)
{
    int xPos = pos.first, yPos = pos.second;
    this->chrboard[xPos][yPos] = symbol;
    this->numboard[xPos][yPos] = id;
}

void BoardStatus::modify(std::pair<int, int> pos, int id)
{
    int xPos = pos.first, yPos = pos.second;
    this->numboard[xPos][yPos] = id;
}