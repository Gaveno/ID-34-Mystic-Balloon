#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"

boolean nextLevelIsVisible;
boolean canPressButton;



void stateMenuPlay()
{
  level = LEVEL_TO_START_WITH - 1;
  coinsCollected = 0;
  balloonsLeft = 0;
  scorePlayer = 0;
  globalCounter = 0;
  gameState = STATE_GAME_NEXT_LEVEL;
  scoreIsVisible = false;
  nextLevelIsVisible = true;
}


void stateGameNextLevel()
{
  if (level < TOTAL_LEVELS)
  {
    if (arduboy.everyXFrames(20))
    {
      canPressButton = false;
      if (coinsCollected > 0)
      {
        coinsCollected--;
        scorePlayer += 20;
      }
      else if (balloonsLeft > 0)
      {
        balloonsLeft--;
        scorePlayer += 20;
      }
      else canPressButton = true;
      /*if (!balloonsLeft && !coinsCollected)
      {
        canPressButton = true;
      }*/
    }
  }
  else gameState = STATE_GAME_OVER;


  if (scoreIsVisible)
  {
    for (byte i = 0; i < coinsCollected; i++) sprites.drawPlusMask(35 + (i * 16), 0, elements, 0);
    for (byte i = 0; i < balloonsLeft; i++) sprites.drawPlusMask(35 + (i * 16), 16, balloon_plus_mask, 0);
    drawNumbers(43, 55, FONT_BIG, DATA_SCORE);
  }

  if (nextLevelIsVisible)
  {
    sprites.drawSelfMasked(35, 31, badgeNextLevel, 0);
    drawNumbers(78, 40, FONT_BIG, DATA_LEVEL);
  }

  if (arduboy.justPressed(A_BUTTON | B_BUTTON) && canPressButton)
  {
    setKid();
    cam.pos = vec2(0, 0);
    cam.offset = vec2(0, 0);
    enemiesInit();
    levelLoad(levels[level]);
    gameState = STATE_GAME_PLAYING;
  }
};


void stateGamePlaying()
{
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
  sprites.drawSelfMasked(47, 15, badgePause, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
  }
}


void stateGameOver()
{
  sprites.drawSelfMasked(47, 15, badgeGameOver, 0);
  drawNumbers(43, 40, FONT_BIG, DATA_SCORE);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}

#endif
