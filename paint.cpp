#include "paint.h"

extern LOGFONT chessfont;

// a better version of solidcircle()
void bettersolidcircle(int x, int y, int radius)
{
    LINESTYLE __linestyle;
    getlinestyle(&__linestyle);
    COLORREF __linecolor = getlinecolor();          // copy origin line style and color

    setlinestyle(PS_SOLID, 1);
    setlinecolor(getfillcolor());

    circle(x, y, radius);
    floodfill(x, y, getfillcolor());

    setlinestyle(&__linestyle);
    setlinecolor(__linecolor);                      // reset to orgin
}

// draw chess board, seam and dest is about how those "target"s perform.
void drawboard(const int seam, const int dest)
{
    LINESTYLE __linestyle;
    getlinestyle(&__linestyle);
    COLORREF __linecolor = getlinecolor();                          // copy origin line style & color
    COLORREF __fillcolor = getfillcolor();                          // copy origin fill color

    setlinestyle(PS_SOLID, GridWeight);
    setlinecolor(GRIDCOLOR);
    setfillcolor(BKGCOLOR);

    solidrectangle(P(0), P(0), P(10), P(11));                       // clear board

    for (int j = 1; j <= 10; j++) {
        line(P(1), P(j), P(9), P(j));
    }
    for (int i = 1; i <= 9; i++) {
        line(P(i), P(1), P(i), P(10));
    }                                                               // draw basic board grid
    solidrectangle(P(1) + 1, P(5) + 1, P(9) - 1, P(6) - 1);         // erase lines that are over Chuhe Hanjie
    line(P(4), P(1), P(6), P(3));
    line(P(6), P(1), P(4), P(3));
    line(P(4), P(10), P(6), P(8));
    line(P(6), P(10), P(4), P(8));                                  // draw diagnals of the Palace

    int targets[][2] = {                                            // positions of each "target" on Canon and Pawn
        {2, 3}, {8, 3}, {1, 4}, {3, 4}, {5, 4}, {7, 4}, {9, 4},
        {2, 8}, {8, 8}, {1, 7}, {3, 7}, {5, 7}, {7, 7}, {9, 7}
    };
    int sgnlist[][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for (auto target : targets) {                                   // for target in targets: for pos in corners: line(vrtc); line(hrzn)
        int x = P(target[0]), y = P(target[1]);
        for (auto sgn : sgnlist) {
            line(x + sgn[0] * seam, y + sgn[1] * seam, x + sgn[0] * dest, y + sgn[1] * seam);       // draw a target that weight is 1
            line(x + sgn[0] * seam, y + sgn[1] * seam, x + sgn[0] * seam, y + sgn[1] * dest);
            x += sgn[0]; y += sgn[1];
            line(x + sgn[0] * seam, y + sgn[1] * seam, x + sgn[0] * (dest - 1), y + sgn[1] * seam); // draw another further to target center
            line(x + sgn[0] * seam, y + sgn[1] * seam, x + sgn[0] * seam, y + sgn[1] * (dest - 1));
            x -= sgn[0]; y -= sgn[1];
        }
    }
    setlinestyle(PS_SOLID, 18);
    setlinecolor(BKGCOLOR);
    rectangle(P(1) - 9, P(1) - 9, P(9) + 10, P(10) + 10);           // erase out-of-range target halves

    setlinestyle(PS_SOLID, 3);
    setlinecolor(GRIDCOLOR);
    rectangle(P(1) - 5, P(1) - 5, P(9) + 6, P(10) + 6);             // draw bold border

    setlinestyle(&__linestyle);
    setlinecolor(__linecolor);
    setfillcolor(__fillcolor);                                      // reset to origin
}

// draw a chess piece at position (x, y)
void drawpiece(int x, int y, COLORREF color, const wchar_t* symbol)
{
    assert(x > PieceRadius && y > PieceRadius && x + PieceRadius < WindowWidth&& y + PieceRadius < WindowHeight);

    COLORREF __fillcolor = getfillcolor();                      // copy origin fill color
    COLORREF __bkcolor = getbkcolor();                          // copy origin background color
    LOGFONT __textstyle;
    gettextstyle(&__textstyle);
    COLORREF __textcolor = gettextcolor();                      // copy origin text style & color
    LINESTYLE __linestyle;
    getlinestyle(&__linestyle);
    COLORREF __linecolor = getlinecolor();                      // copy origin line style & color

    setfillcolor(CHESSCOLOR);
    setbkcolor(CHESSCOLOR);
    settextstyle(&chessfont);
    settextcolor(color);
    setlinestyle(PS_SOLID, std::round(PieceRadius * 0.075));
    setlinecolor(color);

    bettersolidcircle(x, y, PieceRadius);
    outtextxy(x - SymbolSize / 2, y - SymbolSize / 2, symbol);
    circle(x, y, std::round(PieceRadius * 0.9));

    setfillcolor(__fillcolor);
    setbkcolor(__bkcolor);
    settextstyle(&__textstyle);
    settextcolor(__textcolor);
    setlinestyle(&__linestyle);
    setlinecolor(__linecolor);                                  // reset to origin
}

// draw a chess piece whose at board grid (xPos, yPos)
void drawpiece(int x, int y, char symbolchar)
{
    COLORREF color = std::islower(symbolchar) ? BLACKCOLOR : REDCOLOR;

    if (symbolchar == 'M') drawpiece(y, x, LIGHTBLUE, _MUSHROOM);

    else if (symbolchar == 'G') drawpiece(y, x, color, _Governer);
    else if (symbolchar == 'A') drawpiece(y, x, color, _Advisor);
    else if (symbolchar == 'B') drawpiece(y, x, color, _Bishop);
    else if (symbolchar == 'H') drawpiece(y, x, color, _Horse);
    else if (symbolchar == 'R') drawpiece(y, x, color, _Rook);
    else if (symbolchar == 'C') drawpiece(y, x, color, _Cannon);
    else if (symbolchar == 'P') drawpiece(y, x, color, _Pawn);
    else if (symbolchar == 'g') drawpiece(y, x, color, _general);
    else if (symbolchar == 'a') drawpiece(y, x, color, _advisor);
    else if (symbolchar == 'b') drawpiece(y, x, color, _bishop);
    else if (symbolchar == 'h') drawpiece(y, x, color, _horse);
    else if (symbolchar == 'r') drawpiece(y, x, color, _rook);
    else if (symbolchar == 'c') drawpiece(y, x, color, _catapult);
    else if (symbolchar == 'p') drawpiece(y, x, color, _pawn);
}

// draw THE piece
void drawpiece(Piece piece)
{
    drawpiece(P(piece.xPos), P(piece.yPos), piece.name);
}