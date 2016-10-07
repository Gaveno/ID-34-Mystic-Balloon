/*
 Mystic Balloon: http://www.team-arg.org/mybl-manual.html

 Arduboy version 1.2:  http://www.team-arg.org/mybl-downloads.html

 MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

 2016 - GAVENO - CastPixel

 Game License: MIT : https://opensource.org/licenses/MIT

 */

//determine the game
#define GAME_ID 34


#include "Arglib.h"
#include "globals.h"
#include "menu.h"
#include "game.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"


typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM  mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlaySelect,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameNextLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
  stateMenuPlayContinue,
  stateMenuPlayNew,
};

void setup()
{
  arduboy.start();
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
  // EEPROM initialization
  //byte id;
  //EEPROM.get(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START, id);
  if (EEPROM.read(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START) != GAME_ID)
  {
    //id = GAME_ID;
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START, (byte)GAME_ID); // game id
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START + OFFSET_LEVEL, (byte)LEVEL_TO_START_WITH - 1); // beginning level
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START + OFFSET_SCORE, (unsigned long)0); // clear score
    EEPROM.put(EEPROM_STORAGE_SPACE_START + EEPROM_MYSTIC_START + OFFSET_HSCORE, (unsigned long)0); // clear high score
  }
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.poll();
  arduboy.clearDisplay();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}

