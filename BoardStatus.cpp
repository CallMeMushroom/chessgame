#include "BoardStatus.h"

int BoardStatus::getid(std::pair<int, int> pos)
{
    return this->numboard[pos.first][pos.second];
}

char BoardStatus::getsymb(std::pair<int, int> pos)
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

bool BoardStatus::importfrom(std::string str) {
    if (str.length() != 90) {
        std::cout << "Import failed. Length must be exactly 90." << std::endl;
        return false;
    }
    
    std::string pattern = "RHBAGCP rhbagcp";
    for (char ch : str)
        if (!std::count(pattern.begin(), pattern.end(), ch)) {
            std::cout << "Import Failed. Illegal piece(s) found." << std::endl;
            return false;
        }

    for (char ch : {'G', 'g'})
        if (std::count(str.begin(), str.end(), ch) != 1) {
            std::cout << "Import Failed. Both sides must own their Governor/general." << std::endl;
            std::cout << std::count(str.begin(), str.end(), ch) << std::endl;
            return false;
        }

    int pos = 0;
    for (int row = 1; row <= 10; row++)
        for (int column = 1; column <= 9; column++) {
            char ch = str[pos++];
            this->chrboard[row][column] = ch;
        }
    return true;
}