#include "util.h"

// sqaure of distance from (x0, y0) to (x, y), pure math
int distsquare(int x, int y, int x0, int y0) {
    return (x - x0) * (x - x0) + (y - y0) * (y - y0);
}

// are chess A and chess B the same color
bool isfriend(char A, char B) {
    return (std::islower(A) and std::islower(B) or std::isupper(A) and std::isupper(B));
}

// is the mouse in a circle area of the intersection
bool ischoosing(MOUSEMSG mouse, int xPos, int yPos) {
    if (xPos < 1 or xPos > 10 or yPos < 1 or yPos > 9) return false;

    int x = mouse.x, x0 = P(yPos);
    int y = mouse.y, y0 = P(xPos);
    return (distsquare(x, y, x0, y0) <= (PieceRadius + 1) * (PieceRadius + 1));
}