#include "game.h"
#include "Piece.h"
#include "BoardStatus.h"
#include "util.h"
#include "paint.h"

#pragma comment(lib, "winmm.lib")

LOGFONT chessfont;
std::set<std::pair<int, int>> intersections;
std::set<std::pair<int, int>> palace;

BoardStatus board;
std::map<int, Piece> pieces;

std::vector<std::pair<BoardStatus, std::map<int, Piece>>> history;

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

    int id = 1;
    for (auto intersection : intersections) {                               // initialize pieces
        if (board.chrquery(intersection) == ' ') continue;
        board.modify(intersection, id++);
        Piece piece(board, intersection);
        //piece.maintain(board);
        pieces[piece.id] = piece;
    }
    pieces.erase(0);

    history.clear();
    history.push_back(std::make_pair(board, pieces));
}

int main()
{
    assert(WindowWidth >= P(10) and WindowHeight >= P(11));
    initgraph(WindowWidth, WindowHeight, SHOWCONSOLE);
    setbkcolor(BKGCOLOR);
    BeginBatchDraw();
    cleardevice();
    init();

    MOUSEMSG mouse;
    bool shall_render = true;
    bool shall_judge = false;
    int sound = 0;
    Piece hold;                                                             // the chess you are holding (if you are holding one)
    auto save = history;

    while (true) {
        if (shall_render) {
            render();
            shall_render = false;
            FlushBatchDraw();
            if (shall_judge) std::cout << "Shall judge..." << std::endl;
        }

        if (shall_judge) {
            bool checkmate = true;
            for (auto map : pieces) {
                if (!checkmate) break;
                Piece your_piece = map.second;
                if (your_piece.region == board.turnflag)                        // no matter which piece it is...
                    for (auto your_move : your_piece.legalmoves) {              // no matter what move it operates...
                        BoardStatus boardcopy = board;
                        auto piecescopy = pieces;

                        if (boardcopy.chrquery(your_move) != ' ')
                            piecescopy.erase(boardcopy.numquery(your_move));
                        piecescopy[your_piece.id].setpos(your_move);

                        boardcopy.modify(your_piece.getpos(), ' ', 0);
                        boardcopy.modify(your_move, your_piece.name, your_piece.id);

                        bool is_legal = true;
                        for (auto map_ : piecescopy) {
                            if (!is_legal) break;
                            Piece enemy_piece = map_.second;
                            if (enemy_piece.region != boardcopy.turnflag) {         // there's always an enemy ...
                                enemy_piece.maintain(boardcopy);
                                for (auto enemy_move : enemy_piece.legalmoves)      // who's challenging your king.
                                    if (enemy_move == piecescopy[boardcopy.turnflag ? 5 : 28].getpos()) {
                                        is_legal = false;
                                        break;
                                    }
                            }
                        }
                        if (is_legal) {
                            std::cout << your_piece.name << your_move.first << your_move.second << std::endl;
                            checkmate = false;
                            break;
                        }
                    }
            }
            if (checkmate) {
                std::cout << (board.turnflag ? "RED" : "BLACK") << " WINS" << std::endl;
                break;
            }
            shall_judge = false;
            std::cout << "Judged." << std::endl;
        }

        if (!MUTE and sound > 0) {
            mciSendString(_T("close sound"), NULL, 0, NULL);

            switch (sound)
            {
            case CLICK_SOUND:
                mciSendString(_T("open \".\\SE\\CLICK.WAV\" type mpegvideo alias sound"), NULL, 0, NULL);
            case MOVE_SOUND:
                mciSendString(_T("open \".\\SE\\MOVE.WAV\" type mpegvideo alias sound"), NULL, 0, NULL);
            case CAPTURE_SOUND:
                mciSendString(_T("open \".\\SE\\CAPTURE.WAV\" type mpegvideo alias sound"), NULL, 0, NULL);
            case CHECK_SOUND:
                mciSendString(_T("open \".\\SE\\CHECK.WAV\" type mpegvideo alias sound"), NULL, 0, NULL);
            case ILLEGAL_SOUND:
                mciSendString(_T("open \".\\SE\\ILLEGAL.WAV\" type mpegvideo alias sound"), NULL, 0, NULL);
            }
            mciSendString(_T("play sound"), NULL, 0, NULL);
            sound = 0;
        }

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 26) {                                                 // Ctrl+Z ~ rollback
                if (history.size() == 1) continue;
                history.pop_back();
                board = history.back().first;
                pieces = history.back().second;
                shall_render = true;
                std::cout << "Successfully rollback." << std::endl;
            }
            if (ch == 25) {                                                 // Ctrl+Y ~ Sudo
                std::cout << "Sudone." << std::endl;
            }
            else if (ch == 19) {                                            // Ctrl+S ~ save
                save = history;
                std::cout << "Board saved." << std::endl;
            }
            else if (ch == 12) {                                            // Ctrl+L ~ load
                history = save;
                board = history.back().first;
                pieces = history.back().second;
                shall_render = true;
                std::cout << "Board loaded." << std::endl;
            }
            else if (ch == 18) {                                            // Ctrl+R ~ restart
                board = history[0].first;
                pieces = history[0].second;
                history.push_back(std::make_pair(board, pieces));
                shall_render = true;
                std::cout << "Restarted." << std::endl;
            }
            else if (ch == 27 or ch == 23) {                                // Ctrl+W or Escape ~ exit
                std::cout << "Exited." << std::endl;
                break;
            }
            continue;
        }

        if (MouseHit()) {
            mouse = GetMouseMsg();

            if (mouse.uMsg == WM_MBUTTONDOWN) {
                if (mouse.mkCtrl) break;
            }

            else if (mouse.uMsg == WM_RBUTTONDOWN) {                        // clear hold, cancel all choosings
                if (board.choosings.size() > 0) shall_render = true;
                hold.clear();
                board.choosings.clear();
            }

            else if (mouse.uMsg == WM_LBUTTONDOWN) {                        // when left-click
                int xPos = std::round(mouse.y / (double)GridSize);
                int yPos = std::round(mouse.x / (double)GridSize);          // get where you are hovering on
                std::pair<int, int> intersection = std::make_pair(xPos, yPos);
                if (!ischoosing(mouse, xPos, yPos)) continue;

                Piece tmp_piece = pieces[board.numquery(intersection)];     // piece at target intersection

                if (tmp_piece.name != ' ' and tmp_piece.region == board.turnflag) {     // pick up
                    board.choosings.clear();
                    board.choosings.insert(intersection);                   // update
                    hold = tmp_piece;
                    shall_render = true;
                    sound = CLICK_SOUND;
                }

                else if (hold.name != ' ') {                                // put down
                    if (!hold.legalmoves.count(intersection)) continue;

                    BoardStatus boardcopy = board;                          // copy orgin
                    auto piecescopy = pieces;
                    bool capture = false;
                    Piece captured;

                    if (board.chrquery(intersection) != ' ') {
                        captured = pieces[board.numquery(intersection)];
                        capture = true;
                        pieces.erase(board.numquery(intersection));         // is captured
                    }
                    pieces[hold.id].setpos(intersection);                   // modify pieces

                    board.modify(hold.getpos(), ' ', 0);
                    board.modify(intersection, hold.name, hold.id);
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
                        if (piece.region != board.turnflag) {
                            piece.maintain(board);
                            if (piece.legalmoves.count(pieces[board.turnflag ? 5 : 28].getpos())) {
                                in_check = true;                            // if any enemy piece is checking king => illegal move
                                break;
                            }
                        }
                    }

                    if (in_check or face_each) {                            // move leads to be in check => move is illegal
                        board = boardcopy;
                        pieces = piecescopy;
                        sound = ILLEGAL_SOUND;
                        continue;
                    }

                    bool check = false;
                    for (auto map : pieces) {
                        Piece piece = map.second;
                        if (piece.xPos + piece.yPos == 0) continue;
                        if (piece.region == board.turnflag) {
                            piece.maintain(board);
                            if (piece.legalmoves.count(pieces[board.turnflag ? 28 : 5].getpos())) {
                                check = true;
                                break;
                            }
                        }
                    }
                    sound = check ? CHECK_SOUND : (capture ? CAPTURE_SOUND : MOVE_SOUND);

                    if (ANIMATION and ANIME_DUR_SEC > 0) {
                        Piece tmp_piece = pieces[hold.id];
                        pieces.erase(hold.id);                              // remove the real chess piece from board
                        pieces[captured.id] = captured;
                        double dist = std::sqrt(distsquare(hold.getpos(), intersection));
                        double dist_per_frame = dist / (ANIME_DUR_SEC * ANIME_FREQ_HZ);
                        double x = hold.xPos * GridSize, x_to = xPos * GridSize;
                        double y = hold.yPos * GridSize, y_to = yPos * GridSize;
                        long double x_per_frame = (x_to - x) * dist_per_frame / dist;
                        long double y_per_frame = (y_to - y) * dist_per_frame / dist;

                        x += x_per_frame; y += y_per_frame;                 // move by (xpf, ypf) each frame
                        while (x_per_frame * (x_to - x) >= 0 and y_per_frame * (y_to - y) >= 0) {
                            render();
                            FlushBatchDraw();
                            Sleep(100 / ANIME_FREQ_HZ);
                            drawpiece(std::round(x), std::round(y), hold.name);     // draw the "moving" piece
                            FlushBatchDraw();
                            x += x_per_frame; y += y_per_frame;
                        }
                        pieces[hold.id] = tmp_piece;
                        pieces.erase(captured.id);                          // give back the real piece to board
                    }
                    hold.clear();                                           // move is perfectly legal. ready for next move
                    board.turnflag = !board.turnflag;
                    shall_render = true;
                    shall_judge = true;

                    history.push_back(std::make_pair(board, pieces));       // add to history
                }
            }
            else continue;
        }
    }

    pieces[5].name = pieces[28].name = 'M';
    render();
    FlushBatchDraw();
    std::cout << "  Press any key to exit." << std::endl;
    _getch();
    closegraph();
    return 0;
}
//end