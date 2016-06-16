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
#define STATE_GAME_PREPARE_LEVEL     6
#define STATE_GAME_NEXT_LEVEL        7
#define STATE_GAME_PLAYING           8
#define STATE_GAME_PAUSE             9
#define STATE_GAME_OVER              10

#define GAME_TOP                     0
#define GAME_BOTTOM                  64
#define GAME_LEFT                    0
#define GAME_RIGHT                   128

#define SCORE_SMALL_FONT 0
#define SCORE_BIG_FONT 1

#define FACING_RIGHT                 0
#define FACING_LEFT                  1

#define LEVEL_TO_START_WITH          0
#define TOTAL_LEVELS                 5

#define MAX_PER_TYPE                 6 // total instances per enemy type

#define LEVEL_WIDTH 384    // 24 * 16
#define LEVEL_HEIGHT 384   // 24 * 16
#define LEVEL_CELLSIZE 16
#define LEVEL_WIDTH_CELLS 24
#define LEVEL_HEIGHT_CELLS 24
#define LEVEL_CELL_BYTES (LEVEL_WIDTH_CELLS * LEVEL_HEIGHT_CELLS) >> 3
#define LEVEL_ARRAY_SIZE 576

Arduboy arduboy;
Sprites sprites(arduboy);

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;
byte level = LEVEL_TO_START_WITH;
unsigned long scorePlayer;
byte timeBonus;

byte walkerFrame = 0;
byte fanFrame = 0;
byte coinFrame = 0;
byte coinsActive = 0;
Rect levelExit = {.x = 2 << 4, .y = 10 << 4, .width = 16, .height = 16};
vec2 startPos;
byte mapTimer = 10;
//vec2 fanParticles[5];

#endif
