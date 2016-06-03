/*
 Mystic Balloon: http://www.team-arg.org/mybl-manual.html

 Arduboy version 0.1:  http://www.team-arg.org/mybl-downloads.html

 MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

 2016 - GEVANO - CastPixel

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

FunctionPointer mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGamePrepareLevel,
  stateGameNextLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};

void setup()
{
  arduboy.start();
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
  gameState = STATE_MENU_INTRO;                             // start the game with the TEAM a.r.g. logo
  menuSelection = STATE_MENU_PLAY;                          // PLAY menu item is pre-selected
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF)) soundYesNo = true;  // check EEPROM if sound is OFF or ON
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.poll();
  if (soundYesNo == true) arduboy.audio.on();
  else arduboy.audio.off();
  //arduboy.clearDisplay();
  arduboy.fillScreen(gameState == STATE_GAME_PLAYING);
  mainGameLoop[gameState]();
  arduboy.display();
}

