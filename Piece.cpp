#include "Piece.h"

extern std::set<std::pair<int, int>> intersections;
extern std::set<std::pair<int, int>> palace;

std::pair<int, int> Piece::getpos() {
    return std::make_pair(this->xPos, this->yPos);
}

void Piece::setpos(int xPos, int yPos) {
    this->xPos = xPos;
    this->yPos = yPos;
}

void Piece::setpos(std::pair<int, int> pos) {
    this->xPos = pos.first;
    this->yPos = pos.second;
}

void Piece::clear() {
    this->xPos = 0; this->yPos = 0;
    this->name = ' ';
    this->id = 0;
    this->region = true;
    this->legalmoves.clear();
}

// whether the given pos is in reach of this piece of this board
bool Piece::inreach(BoardStatus board, int xPos, int yPos) {
    int xDist = xPos - this->xPos, yDist = yPos - this->yPos;
    bool crossriver = ((xPos / 6) == !!std::islower(this->name));
    int step = (xDist ^ yDist) > 0 ? 1 : -1;
    std::vector<char> path;

    switch (std::toupper(this->name)) {
    case 'G':
        if (abs(xDist) + abs(yDist) != 1) return false;
        return (palace.count(std::make_pair(xPos, yPos)));

    case 'A':
        if (abs(xDist) != 1 or abs(yDist) != 1) return false;
        return (palace.count(std::make_pair(xPos, yPos)));

    case 'B':
        if (crossriver) return false;
        if (abs(xDist) != 2 or abs(yDist) != 2) return false;
        return (board.chrboard[this->xPos + xDist / 2][this->yPos + yDist / 2] == ' ');

    case 'H':
        if (abs(xDist * yDist) != 2) return false;
        return (board.chrboard[this->xPos + xDist / 2][this->yPos + yDist / 2] == ' ');
        // comment: I'm really surprised that Bishops and Knights have formula of blockage exactly THE SAME!!!

    case 'R':
        if (xDist * yDist != 0) return false;
        if (xDist) {
            for (int row = this->xPos + step; row != xPos; row += step)
                if (board.chrboard[row][yPos] != ' ') return false;
        }
        else if (yDist) {
            for (int clm = this->yPos + step; clm != yPos; clm += step)
                if (board.chrboard[xPos][clm] != ' ') return false;
        }
        return true;

    case 'C':
        if (xDist * yDist != 0) return false;

        if (xDist) {
            for (int row = this->xPos + step; row != xPos + step; row += step)
                if (board.chrboard[row][yPos] != ' ')
                    path.push_back(board.chrboard[row][yPos]);
        }
        else if (yDist) {
            for (int clm = this->yPos + step; clm != yPos + step; clm += step)
                if (board.chrboard[xPos][clm] != ' ')
                    path.push_back(board.chrboard[xPos][clm]);
        }
        if (path.size() != 0 and path.size() != 2) return false;
        return (path.size() == 0) == (board.chrboard[xPos][yPos] == ' ');
        // comment: rule code for Rook and Canon is super ugly, looking forward to simplification.

    case 'P':
        if (crossriver and abs(yDist) == 1 and xDist == 0) return true;
        return (xDist == (std::islower(this->name) ? 1 : -1) and yDist == 0);
    }
}

bool Piece::inreach(BoardStatus board, std::pair<int, int> pos) {
    return this->inreach(board, pos.first, pos.second);
}

void Piece::maintain(BoardStatus board) {
    this->legalmoves.clear();
    for (auto intersection : intersections) {
        if (!isfriend(this->name, board.getsymb(intersection)) and this->inreach(board, intersection))
            this->legalmoves.insert(intersection);
    }
}

Piece::Piece() {
    this->xPos = 0; this->yPos = 0;
    this->id = 0;
    this->name = ' ';
    this->region = true;
}

Piece::Piece(BoardStatus board, std::pair<int, int> intersection) {
    this->xPos = intersection.first;
    this->yPos = intersection.second;
    this->name = board.getsymb(intersection);
    this->id = board.getid(intersection);
    this->region = !!std::islower(this->name);
    this->legalmoves.clear();
    this->maintain(board);
}