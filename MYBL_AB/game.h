#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"

void stateGamePrepareLevel()
{
  //level = 0;
  timeBonus = 254;
  setKid();
  cam.pos = vec2(0, 0);
  cam.offset = vec2(0, 0);
  //gameState = STATE_GAME_NEXT_LEVEL;
  gameState = STATE_GAME_PLAYING;
  enemiesInit();
  levelLoad(levels[level]);
};

void stateGameNextLevel()
{
  scorePlayer += (kid.balloons * 100) + timeBonus;
  ++level;
  if (level >= TOTAL_LEVELS)
    gameState = STATE_GAME_OVER;
  else
    gameState = STATE_GAME_PREPARE_LEVEL;
};

void drawHUD()
{
  for (byte i = 0; i< 16; i++)
  {
    sprites.drawSelfMasked(i*8, 0, smallMask, 0);
  }
  drawBalloonLives();
  drawScore(91, 0, 0);
  drawCoinHUD();
  if (key.haveKey) sprites.drawErase(28, 0, elementsHUD, 13);
}

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
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}


void stateGameOver()
{
  drawScore(32, 36, 1);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_INTRO;
  }
}

#endif
