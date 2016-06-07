#ifndef MENU_BITMAPS_H
#define MENU_BITMAPS_H

#include <Arduino.h>
#include "globals.h"

byte blinkingFrames = 0;
byte sparkleFrames = 0;

void drawTitleScreen()
{
  if (arduboy.everyXFrames(1)) blinkingFrames++;
  if (arduboy.everyXFrames(2)) sparkleFrames++;
  if (sparkleFrames > 4) sparkleFrames = 0;
  if (blinkingFrames > 31) blinkingFrames = 0;
  arduboy.drawCompressed(0, 0, titleScreen, WHITE);
  arduboy.drawCompressed(85, 45, mysticBalloon, WHITE);
  sprites.drawSelfMasked(79, 43, stars, sparkleFrames);
  sprites.drawSelfMasked(9, 9, leftGuyLeftEye, pgm_read_byte(&blinkingEyesLeftGuy[blinkingFrames]));
  sprites.drawSelfMasked(15, 13, leftGuyRightEye, pgm_read_byte(&blinkingEyesLeftGuy[blinkingFrames]));
  sprites.drawSelfMasked(109, 34, rightGuyEyes, pgm_read_byte(&blinkingEyesRightGuy[blinkingFrames]));
}

void stateMenuIntro()
{
  counter++;
  if (counter < 46) arduboy.drawCompressed(0, 0, TEAMarg, WHITE);
  if (counter > 45) drawTitleScreen();
  if ((counter > 90) || arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuMain()
{

  drawTitleScreen();
  arduboy.drawCompressed(51, 9, mainMenuMask, BLACK);
  arduboy.drawCompressed(51, 9, mainMenu, WHITE);
  if (arduboy.justPressed(DOWN_BUTTON) && (menuSelection < 5)) menuSelection++;
  if (arduboy.justPressed(UP_BUTTON) && (menuSelection > 2)) menuSelection--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = menuSelection;
  arduboy.drawCompressed(46, 9 + 9 * (menuSelection - 2), selectorMask, BLACK);
  arduboy.drawCompressed(46, 9 + 9 * (menuSelection - 2), selector, WHITE);
}

void stateMenuHelp()
{
  arduboy.drawCompressed(32, 0, qrcode, WHITE);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuPlay()
{
  level = 1;
  gameState = STATE_GAME_PREPARE_LEVEL;
}

void stateMenuInfo()
{
  if (arduboy.everyXFrames(10)) sparkleFrames++;
  if (sparkleFrames > 4) sparkleFrames = 0;
  arduboy.drawCompressed(43, 23, mysticBalloon, WHITE);
  sprites.drawSelfMasked(37, 21, stars, sparkleFrames);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  drawTitleScreen();
  arduboy.drawCompressed(51, 9, soundMenuMask, BLACK);
  arduboy.drawCompressed(51, 9, soundMenu, WHITE);
  if (arduboy.justPressed(DOWN_BUTTON)) soundYesNo = true;
  if (arduboy.justPressed(UP_BUTTON)) soundYesNo = false;
  arduboy.drawCompressed(54, 18 + 9 * soundYesNo, selectorMask, BLACK);
  arduboy.drawCompressed(54, 18 + 9 * soundYesNo, selector, WHITE);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
  if (soundYesNo == true) arduboy.audio.on();
  else arduboy.audio.off();
}


#endif
