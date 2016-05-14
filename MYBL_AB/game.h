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
  level = 0;
  scorePlayer = 0;
  setKid();
  gameState = STATE_GAME_NEXT_LEVEL;
};

void stateGameNextLevel()
{
  level++;
  gameState = STATE_GAME_PLAYING;
};

void stateGamePlaying()
{
  checkInputs();
  checkKid();

  drawKid();
  
  checkCollisions();
}


void stateGamePause()
{
  if (buttons.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}


void stateGameOver()
{
  if (buttons.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
  }
}

#endif
