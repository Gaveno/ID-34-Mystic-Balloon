/*
 Mystic Balloon: http://www.team-arg.org/mybl-manual.html

 Arduboy version 0.1:  http://www.team-arg.org/mybl-downloads.html

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
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameNextLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
  stateGameEnd,
};

void setup()
{
  arduboy.start();
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.poll();
  arduboy.clearDisplay();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}

