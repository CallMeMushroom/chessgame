#include "BoardStatus.h"

int BoardStatus::numquery(std::pair<int, int> pos) {
    return this->numboard[pos.first][pos.second];
}

char BoardStatus::chrquery(std::pair<int, int> pos) {
    return this->chrboard[pos.first][pos.second];
}