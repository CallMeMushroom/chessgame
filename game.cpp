#include "game.h"
#include "Piece.h"
#include "BoardStatus.h"
#include "util.h"
#include "paint.h"

LOGFONT chessfont;
std::set<std::pair<int, int>> intersections;
std::set<std::pair<int, int>> palace;

BoardStatus board;
std::map<int, Piece> pieces;

// show board and all chess pieces and chosen intersection(s)
void render() {
    drawboard();

    int size = PieceRadius / 2;
    int sgnlist[][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for (auto choose : board.choosings) {                                         // draw focuses
        int centre_x = P(choose.second), centre_y = P(choose.first);
        for (auto sgn : sgnlist) {
            int x = centre_x + sgn[0] * size;
            int y = centre_y + sgn[1] * size;
            for (int i = x + sgn[0] * size; i != x; i -= sgn[0])
                for (int j = y + sgn[1] * size; j != y; j -= sgn[1])
                    if (distsquare(x, y, i, j) > size * size + 4)
                        putpixel(i, j, FOCUSCOLOR);
                    else break;
        }
    }

    for (auto map : pieces) drawpiece(map.second);
}

// everything you shall do before run anything
void init() {
    gettextstyle(&chessfont);                                               // initialize chessfont
    chessfont.lfHeight = SymbolSize;
    chessfont.lfWeight = FW_BOLD;
    _tcscpy_s(chessfont.lfFaceName, CHESSFONT);
    chessfont.lfQuality = ANTIALIASED_QUALITY;
    //setbkmode(TRANSPARENT);
    for (int row : {1, 2, 3, 8, 9, 10})                                     // initialize palace
        for (int column : {4, 5, 6})
            palace.insert(std::make_pair(row, column));

    for (int row = 1; row <= 10; row++)                                     // initialize intersections
        for (int column = 1; column <= 9; column++)
            intersections.insert(std::make_pair(row, column));

    for (auto intersection : intersections) {                               // initialize pieces
        if (board.numquery(intersection) == 0) continue;
        //Piece piece(board, intersection);
        Piece piece;
        piece.setpos(intersection);
        piece.name = board.chrquery(intersection);
        piece.id = board.numquery(intersection);
        piece.region = !!std::islower(piece.name);
        piece.maintain(board);
        pieces[piece.id] = piece;
    }
    pieces.erase(0);
}

int main()
{
    assert(WindowWidth > P(10) and WindowHeight > P(11));
    initgraph(WindowWidth, WindowHeight, SHOWCONSOLE);
    setbkcolor(BKGCOLOR);
    BeginBatchDraw();
    cleardevice();
    init();

    MOUSEMSG mouse;
    bool shall_render = true;
    bool shall_judge = false;
    bool turnflag = REDTURN;
    bool checkmate = false;
    Piece hold;                                                             // the chess you are holding (if you are holding one)

    while (true) { // for each "frame"
        if (shall_render) {
            render();
            shall_render = false;
            FlushBatchDraw();
        }

        /*
        if (shall_judge)
            for (auto map : pieces) {
                Piece your_piece = map.second;
                if (your_piece.region == turnflag)                          // no matter which piece ...
                    for (auto your_move : your_piece.legalmoves) {          // no matter how to move ...
                        // board.modify();
                        for (auto map_ : pieces) {
                            Piece enemy_piece = map_.second;
                            if (enemy_piece.region != turnflag)             // there's always an enemy ...
                                for (auto enemy_move : enemy_piece.legalmoves)
                                    if (enemy_move == pieces[turnflag ? 5 : 28].getpos()) {
                                        checkmate = true;
                                        break;
                                    }
                        }
                    }
            }
        */

        if (MouseHit()) {
            mouse = GetMouseMsg();

            if (mouse.uMsg == WM_MBUTTONDOWN) {
                break;
            }

            else if (mouse.uMsg == WM_RBUTTONDOWN) {                        // cancel all choosings
                if (board.choosings.size() > 0) shall_render = true;
                hold.clear();
                board.choosings.clear();
            }

            else if (mouse.uMsg == WM_LBUTTONDOWN) {                        // when left-click
                int xPos = std::round(mouse.y / (double)GridSize);
                int yPos = std::round(mouse.x / (double)GridSize);          // get where you are hovering on
                std::pair<int, int> intersection = std::make_pair(xPos, yPos);
                if (!ischoosing(mouse, xPos, yPos)) continue;

                Piece tmp_piece = pieces[board.numquery(intersection)];         // piece at target intersection

                if (tmp_piece.name != ' ' and tmp_piece.region == turnflag) {       // pick up a piece
                    board.choosings.clear();
                    board.choosings.insert(intersection);                   // update choosings
                    hold = tmp_piece;                                           // update hold
                    shall_render = true;
                }

                else if (hold.name != ' ') {                                // put down a piece
                    std::cout << hold.name << ' ' << hold.legalmoves.count(intersection);

                    if (!hold.legalmoves.count(intersection)) continue;

                    BoardStatus boardcopy = board;                          // copy orgin
                    auto piecescopy = pieces;

                    if (board.chrquery(intersection) != ' ')
                        pieces.erase(board.numquery(intersection));         // is captured
                    pieces[hold.id].setpos(intersection);                   // modify pieces

                    board.chrboard[hold.xPos][hold.yPos] = ' ';             // modify board
                    board.numboard[hold.xPos][hold.yPos] = 0;
                    board.chrboard[xPos][yPos] = hold.name;
                    board.numboard[xPos][yPos] = hold.id;
                    board.choosings.insert(intersection);                   // modify choosings

                    bool in_check = false;                                  // default: this move does NOT lead to be in check
                    bool face_each = (pieces[5].yPos == pieces[28].yPos);   // default: kings DO face each other, if in same column
                    for (auto& map : pieces) {
                        Piece& piece = map.second;
                        if (piece.xPos + piece.yPos == 0) continue;
                        // TODO: pieces would somehow mysteriously insert a (0, 0)
                        if (face_each and (piece.yPos == pieces[5].yPos))
                            if (pieces[5].xPos < piece.xPos and piece.xPos < pieces[28].xPos)
                                face_each = false;                          // if any piece is between kings => they do NOT face each other.
                        if (piece.region != turnflag) {
                            piece.maintain(board);
                            if (piece.legalmoves.count(pieces[turnflag ? 5 : 28].getpos())) {
                                in_check = true;                            // if any enemy piece is checking king => illegal move
                                break;
                            }
                        }
                    }

                    if (in_check or face_each) {                            // move leads to be in check => move is illegal
                        board = boardcopy;
                        pieces = piecescopy;
                        continue;
                    }

                    hold.clear();                                           // move is perfectly legal. ready for next move
                    turnflag = !turnflag;
                    shall_render = true;
                    shall_judge = true;
                }
            }
            else continue;
        }
    }

    drawpiece(P(5), P(10), LIGHTBLUE, _MUSHROOM);
    drawpiece(P(5), P(1), LIGHTBLUE, _MUSHROOM);
    FlushBatchDraw();
    _getch();
    closegraph();
    return 0;
}
//end