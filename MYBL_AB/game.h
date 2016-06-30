#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"

byte gameOverAndStageFase;
byte balloonsLeft;
byte coinsLeft;
boolean balloonsIsVisible;
boolean scoreIsVisible;
boolean coinsIsVisible;
boolean nextLevelIsVisible;

void nextLevelStart()
{
  nextLevelIsVisible = false;
  coinsIsVisible = false;
  balloonsIsVisible = false;
  nextLevelIsVisible = false;
  
  if (level < 1)
  {
    gameOverAndStageFase = 4;
    nextLevelIsVisible = true;
  }
  else
  {
    balloonsLeft = kid.balloons;
    coinsLeft = 0;
    scoreIsVisible = true;
    gameOverAndStageFase++;
  }
}

void nextLevelWait()
{
  if (arduboy.everyXFrames(4)) globalCounter++;
  if (globalCounter > 8)
  {
    gameOverAndStageFase++;
    globalCounter = 0;
  }
}

void nextLevelBonus()
{
  if (timeBonus > 0)
  {
    timeBonus--;
    scorePlayer += 5;
  }
  else
  {
    if (level < TOTAL_LEVELS)
    {
      if (arduboy.everyXFrames(20) && coinsCollected > 0)
      {
        coinsIsVisible = true;
        coinsLeft++;
        coinsCollected--;
        scorePlayer += 20;
      }
      if (arduboy.everyXFrames(20) && balloonsLeft > 0 && !coinsCollected)
      {
        balloonsIsVisible = true;
        balloonsLeft--;
        scorePlayer += 20;
      }
      if (!balloonsLeft && !coinsCollected) gameOverAndStageFase++;
    }
    else
    {
      gameState = STATE_GAME_OVER;
      gameOverAndStageFase = 0;
    }
  }
}

void nextLevelEnd()
{
  balloonsIsVisible = false;
  coinsIsVisible = false;
  nextLevelIsVisible = true;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    timeBonus = 254;
    setKid();
    cam.pos = vec2(0, 0);
    cam.offset = vec2(0, 0);
    enemiesInit();
    levelLoad(levels[level]);
    gameOverAndStageFase = 0;
    gameState = STATE_GAME_PLAYING;
  }
}

typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM nextLevelFases[] =
{
  nextLevelStart,
  nextLevelWait,
  nextLevelBonus,
  nextLevelWait,
  nextLevelEnd,
};



void stateMenuPlay()
{
  level = LEVEL_TO_START_WITH - 1;
  coinsCollected = 0;
  scorePlayer = 0;
  gameOverAndStageFase = 0;
  globalCounter = 0;
  gameState = STATE_GAME_NEXT_LEVEL;
}


void stateGameNextLevel()
{
  ((FunctionPointer) pgm_read_word (&nextLevelFases[gameOverAndStageFase]))();
  if (scoreIsVisible) drawNumbers(43, 48, FONT_BIG, DATA_SCORE);
  if (balloonsIsVisible) for(byte i=0; i< kid.balloons; i++) sprites.drawPlusMask(35+ (i*16), 20, balloon_plus_mask, 0);
  if (coinsIsVisible) for(byte i=0; i< coinsLeft; i++) sprites.drawPlusMask(35+ (i*16), 4, sprCoin, 0);
  if (nextLevelIsVisible)
  {
    drawNumbers(78, 22, FONT_BIG, DATA_LEVEL);
    sprites.drawSelfMasked(35, 20, badgeNextLevel, 0);
  }
};


void stateGamePlaying()
{
  if (timeBonus > 0 && arduboy.everyXFrames(45)) --timeBonus;
  checkInputs();
  checkKid();
  updateCamera();

  drawGrid();
  enemiesUpdate();

  drawKid();
  drawHUD();

  checkCollisions();
}


void stateGamePause()
{
  sprites.drawSelfMasked(46, 15, badgePause, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
  }
}


void stateGameOver()
{
  sprites.drawSelfMasked(35, 20, badgeGameOver, 0);
  drawNumbers(43, 40, FONT_BIG, DATA_SCORE);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}

#endif
