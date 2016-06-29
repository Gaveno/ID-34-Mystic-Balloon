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
boolean showBonusScore;
boolean showScore;
boolean showCoins;
boolean nextLevelVisible;

void nextLevelStart()
{
  nextLevelVisible = true;
  if (level < 1)
  {
    gameOverAndStageFase = 8;
  }
  if (level > TOTAL_LEVELS - 1)
  {
    gameState = STATE_GAME_END;
    return;
  }
  if (level > 0)
  {
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

void nextLevelTimeBonus()
{
  if (timeBonus > 0)
  {
    timeBonus--;
    scorePlayer += 5;
  }
  else
  {
    if (level < TOTAL_LEVELS) gameOverAndStageFase++;
    else
    {
      gameState = STATE_GAME_END;
      gameOverAndStageFase = 0;
    }
  }
  gameOverAndStageFase++;
}

void nextLevelBalloonBonus()
{
  scorePlayer += (kid.balloons * 50);
}

void nextLevelCoinsBonus()
{
  if (arduboy.everyXFrames(8) && coinsCollected > 0)
  {
    coinsCollected--;
    scorePlayer += 20;
  }
  else gameOverAndStageFase++;
}

void nextLevelEnd()
{
  nextLevelVisible = true;
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
  nextLevelTimeBonus,
  nextLevelWait,
  nextLevelBalloonBonus,
  nextLevelWait,
  nextLevelCoinsBonus,
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
  if (arduboy.everyXFrames(8)) sparkleFrames++;
  if (sparkleFrames > 4) sparkleFrames = 0;
  if (nextLevelVisible)
  {
    sprites.drawSelfMasked(29, 18, stars, sparkleFrames);
    drawNumbers(78, 22, FONT_BIG, DATA_LEVEL);
    sprites.drawSelfMasked(35, 20, badgeNextLevel, 0);
  }
  ((FunctionPointer) pgm_read_word (&nextLevelFases[gameOverAndStageFase]))();
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
  //drawMiniMap(mapTimer);
  //if (mapTimer > 0) mapTimer--;
  /*if (mapTimer == 0)
    drawMiniMap();
    else
    mapTimer--;*/

  checkCollisions();
}


void stateGamePause()
{
  //sprites.drawSelfMasked(46, 15, nextLevel, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
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

void stateGameEnd()
{
  sprites.drawSelfMasked(35, 20, badgeGameOver, 0);
  drawNumbers(32, 36, FONT_BIG, DATA_SCORE);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}

#endif
