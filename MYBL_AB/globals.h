#ifndef GLOBALS_H
#define GLOBALS_H

/*-----------------------------*
   To turn on hard mode
   uncomment the below define.

   Hard mode makes it so you
   start each level without
   recovering balloons.
  ----------------------------*/
//#define HARD_MODE

#include <Arduino.h>
#include "Arglib.h"
#include "vec2.h"
#include "bitmaps.h"

// EEPROM - change this address offset from the arduboy starting address if desired
#define EEPROM_MYSTIC_START          16
#define OFFSET_LEVEL                 1
#define OFFSET_COINS                 2
#define OFFSET_COINSHS               3
#define OFFSET_SCORE                 4
#define OFFSET_HSCORE                4 + sizeof(unsigned long)

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
#define STATE_GAME_PLAYCONTNEW       10 // 11

#define FACING_RIGHT                 0
#define FACING_LEFT                  1

#define LEVEL_TO_START_WITH          1
#define TOTAL_LEVELS                 30
#define TOTAL_COINS                  TOTAL_LEVELS * 6

#define MAX_PER_TYPE                 6                    // total instances per enemy type

#define LEVEL_WIDTH                  384                  // 24 * 16
#define LEVEL_HEIGHT                 384                  // 24 * 16
#define LEVEL_CELLSIZE               16
#define LEVEL_WIDTH_CELLS            24
#define LEVEL_HEIGHT_CELLS           24
#define LEVEL_CELL_BYTES             (LEVEL_WIDTH_CELLS * LEVEL_HEIGHT_CELLS) >> 3
#define LEVEL_ARRAY_SIZE             576

#define PLAYER_JUMP_TIME             11

Arduboy arduboy;
Sprites sprites(arduboy);

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;
byte level;
unsigned long scorePlayer;
byte coinsCollected = 0;
byte totalCoins = 0;
byte balloonsLeft;

boolean nextLevelIsVisible;
boolean scoreIsVisible;
boolean canPressButton;
boolean pressKeyIsVisible;

byte walkerFrame = 0;
byte fanFrame = 0;
byte coinFrame = 0;
byte coinsActive = 0;
vec2 levelExit = vec2(0, 0);
vec2 startPos;
byte mapTimer = 10;

void loadSetEEPROM()
{
  if (EEPROM.read(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START) != GAME_ID)
  {
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START, (byte)GAME_ID); // game id
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START + OFFSET_LEVEL, (byte)LEVEL_TO_START_WITH - 1); // beginning level
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START + OFFSET_COINS, (byte)0); // coins current run
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START + OFFSET_COINSHS, (byte)0); // coins highscore run
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START + OFFSET_SCORE, (unsigned long)0); // clear score
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START + OFFSET_HSCORE, (unsigned long)0); // clear high score
  }
}


#endif
