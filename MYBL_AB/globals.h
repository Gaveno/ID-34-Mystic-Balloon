#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "Arglib.h"
#include "vec2.h"
#include "bitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1
#define STATE_MENU_HELP              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INFO              4
#define STATE_MENU_SOUNDFX           5

//define game states (on main menu)
#define STATE_GAME_NEXT_LEVEL        6
#define STATE_GAME_PLAYING           7
#define STATE_GAME_PAUSE             8
#define STATE_GAME_OVER              9

#define GAME_TOP                     0
#define GAME_BOTTOM                  64
#define GAME_LEFT                    0
#define GAME_RIGHT                   128

#define FACING_RIGHT                 0
#define FACING_LEFT                  1

#define LEVEL_TO_START_WITH          1
#define TOTAL_LEVELS                 12
#define TOTAL_COINS                  TOTAL_LEVELS * 6

#define MAX_PER_TYPE                 6                    // total instances per enemy type

#define LEVEL_WIDTH                  384                  // 24 * 16
#define LEVEL_HEIGHT                 384                  // 24 * 16
#define LEVEL_CELLSIZE               16
#define LEVEL_WIDTH_CELLS            24
#define LEVEL_HEIGHT_CELLS           24
#define LEVEL_CELL_BYTES             (LEVEL_WIDTH_CELLS * LEVEL_HEIGHT_CELLS) >> 3
#define LEVEL_ARRAY_SIZE             576

#define PLAYER_JUMP_TIME             10

Arduboy arduboy;
Sprites sprites(arduboy);

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;
byte level;
unsigned long scorePlayer;
byte coinsCollected = 0;
byte balloonsLeft;
boolean scoreIsVisible;

byte walkerFrame = 0;
byte fanFrame = 0;
byte coinFrame = 0;
byte coinsActive = 0;
Rect levelExit = {.x = 2 << 4, .y = 10 << 4, .width = 16, .height = 16};
vec2 startPos;
byte mapTimer = 10;
//vec2 fanParticles[5];

#endif
