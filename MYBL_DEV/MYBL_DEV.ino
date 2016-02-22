/*
 GAME-NAME: http://www.team-arg.org/mybl-manual.html

 Arduboy version 0.1:  http://www.team-arg.org/mybl-downloads.html

 MADE by TEAM a.r.g. : http://www.team-arg.org/About.html

 2016 - GEVANO - pixelart by CastPixel: https://twitter.com/castpixel

 Game License: MIT : https://opensource.org/licenses/MIT

 */

//determine the game
#define GAME_ID 34

#include <SPI.h>
#include <EEPROM.h>
#include "Arglib.h"
#include "menu.h"
#include "game.h"

Arduboy arduboy;

byte gameState;

typedef void (*FunctionPointer) ();
FunctionPointer mainGameLoop[] =
{
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};

void setup()
{
  arduboy.start();
  arduboy.setFrameRate(60);
  gameState = 0;
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF)) soundYesNo = true;
  menuSelection = 3;
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  buttons.poll();
  if (soundYesNo == true) arduboy.audio.on();
  else arduboy.audio.off();
  arduboy.clearDisplay();
  mainGameLoop[gameState]();
  arduboy.display();
}

