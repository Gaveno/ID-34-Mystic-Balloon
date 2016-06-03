#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "globals.h"
#include "player.h"

void checkInputs()
{
kid.isWalking = false;
  if (arduboy.pressed(DOWN_BUTTON) && (kid.y < GAME_BOTTOM))
  {

  }
  if (arduboy.pressed(LEFT_BUTTON) && (kid.x > GAME_LEFT))
  {
    kid.direction = FACING_LEFT;
    kid.x--;
    if (!kid.isJumping && !kid.isLanding)kid.isWalking = true;
  }
  if (arduboy.pressed(UP_BUTTON) && (kid.y > GAME_TOP))
  {

  }
  if (arduboy.pressed(RIGHT_BUTTON) && (kid.x < GAME_RIGHT))
  {
    kid.direction = FACING_RIGHT;
    kid.x++;
    if (!kid.isJumping && !kid.isLanding)kid.isWalking = true;
  }

  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PAUSE;
  if (arduboy.justPressed(B_BUTTON))
  {
    kid.isWalking = false;
    kid.isJumping = true;
  }
}

#endif
