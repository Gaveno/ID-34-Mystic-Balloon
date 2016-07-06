#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"


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
  pressKeyIsVisible = false;
}


void stateGameNextLevel()
{
  if (level < TOTAL_LEVELS)
  {
    if (arduboy.everyXFrames(30))
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
      else
      {
        canPressButton = true;
        scoreIsVisible = false;
        pressKeyIsVisible = !pressKeyIsVisible;
      }
    }
  }
  else gameState = STATE_GAME_OVER;


  if (nextLevelIsVisible)
  {
    sprites.drawSelfMasked(35, 4, badgeNextLevel, 0);
    drawNumbers(78, 13, FONT_BIG, DATA_LEVEL);
    drawNumbers(43, 49, FONT_BIG, DATA_SCORE);
  }

  if (scoreIsVisible)
  {
    byte totalBadges = coinsCollected + balloonsLeft;

    for (byte i = 0; i < totalBadges; ++i)
    {
      if (i < coinsCollected) sprites.drawOverwrite(65 - (7 * totalBadges) + (i * 14), 27, badgeElements, 0);
      else sprites.drawOverwrite(65 - (7 * totalBadges) + (i * 14), 27, badgeElements, 1);
    }
  }

  if (canPressButton)
  {

    if (pressKeyIsVisible) sprites.drawOverwrite(38, 29, badgePressKey, 0);
    if (arduboy.justPressed(A_BUTTON | B_BUTTON))
    {
      setKid();
      cam.pos = vec2(0, 0);
      cam.offset = vec2(0, 0);
      enemiesInit();
      levelLoad(levels[level]);
      gameState = STATE_GAME_PLAYING;
    }
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
  sprites.drawSelfMasked(47, 17, badgePause, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
  }
}


void stateGameOver()
{
  sprites.drawSelfMasked(35, 17, badgeGameOver, 0);
  drawNumbers(78, 26, FONT_BIG, DATA_LEVEL);
  drawNumbers(43, 49, FONT_BIG, DATA_SCORE);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}

#endif
