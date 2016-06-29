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
  nextLevelVisible = false;
  if (level < 1)
  {
    gameOverAndStageFase = 3;
  }
  if (level > TOTAL_LEVELS - 1)
  {
    gameState = STATE_GAME_OVER;
    return;
  }
  if (level > 0)
  {

    level++;
    gameOverAndStageFase++;
  }
}

void nextLevelTimeBonus()
{
  scorePlayer += (kid.balloons * 100) + timeBonus;
  gameOverAndStageFase++;
}

void nextLevelCoinsBonus()
{
  gameOverAndStageFase++;
}

void nextLevelEnd()
{
  nextLevelVisible = true;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    //level = 0;
    timeBonus = 254;
    setKid();
    cam.pos = vec2(0, 0);
    cam.offset = vec2(0, 0);
    //gameState = STATE_GAME_NEXT_LEVEL;
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
  nextLevelTimeBonus,
  nextLevelCoinsBonus,
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
  drawScore(43, 40, SCORE_BIG_FONT);
  if (nextLevelVisible) sprites.drawSelfMasked(46, 15, badgeNextLevel, 0);
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
  drawScore(43, 40, SCORE_BIG_FONT);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}

void stateGameEnd()
{
  drawScore(32, 36, SCORE_BIG_FONT);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}

#endif
