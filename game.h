#pragma once

#ifndef game_h
#define game_h

#include <iostream>
#include <assert.h>
#include <conio.h>
#include <graphics.h>
#include <cmath>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include <Windows.h>
#include <mmsystem.h>

#define _Governer               _T("\u5e25")            // red King     (or Governer)
#define _Advisor                _T("\u4ed5")            // red Advisor  (or Counselor)
#define _Bishop                 _T("\u76f8")            // red Bishop   (or Minister)
#define _Horse                  _T("\u99ac")            // red Knight   (or Horse)
#define _Rook                   _T("\u8eca")            // red Rook     (or Chariot)
#define _Cannon                 _T("\u70ae")            // red Cannon
#define _Pawn                   _T("\u5175")            // red Pawn     (or Soldier)
#define _general                _T("\u5c07")            // black King   (or General)
#define _advisor                _T("\u58eb")            // black Advisor
#define _bishop                 _T("\u8c61")            // black Bishop (or Elephant)
#define _horse                  _T("\u99ac")            // black Knight (or Horse)
#define _rook                   _T("\u8eca")            // black Rook   (or Chariot)
#define _catapult               _T("\u7832")            // black Catapult
#define _pawn                   _T("\u5352")            // black Pawn
#define _MUSHROOM               _T("\u8611")            // CALLMEMUSHROOM! >_<

#define GRAY                    0x7F7F7F
#define OFFWHITE                0xD6EEF7
#define CRIMSON                 0x462FBF
#define NAVAJO                  0xADDEFF
#define MOCCASIN                0xB5E4FF
#define BRULYWOOD               0x87B8DE

#define REDTURN                 false
#define BLACKTURN               true

#define P(pos)                  (pos) * GridSize

#define SymbolSize              36                  // SymbolSize *MUST* be an even integer!
#define CHESSFONT               _T("HanWangWeBe")

constexpr int PieceRadius = (int)(1.414 * SymbolSize / 2) + 2;
constexpr int GridSize = (int)(PieceRadius * 2.22) + 1;
constexpr int GridWeight = (int)(GridSize / 50);

#define WindowWidth             P(10)
#define WindowHeight            P(11)

#define BKGCOLOR                MOCCASIN
#define CHESSCOLOR              BRULYWOOD
#define REDCOLOR                CRIMSON
#define BLACKCOLOR              BLACK
#define GRIDCOLOR               BLACK
#define TEXTCOLOR               BLACK
#define FOCUSCOLOR              BLUE

#define MUTE                    false
#define CLICK_SOUND             1
#define MOVE_SOUND              2
#define CAPTURE_SOUND           3
#define CHECK_SOUND             4
#define ILLEGAL_SOUND           5
#define END_GAME_SOUND          6

#define ANIMATION               true
#define ANIME_FREQ_HZ           180
#define ANIME_DUR_SEC           0.075

#endif