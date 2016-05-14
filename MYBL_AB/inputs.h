#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "globals.h"
#include "player.h"

void checkInputs()
{
kid.walking = false;
  if (buttons.pressed(DOWN_BUTTON) && (kid.y < GAME_BOTTOM))
  {

  }
  if (buttons.pressed(LEFT_BUTTON) && (kid.x > GAME_LEFT))
  {
    kid.direction = FACING_LEFT;
    kid.x--;
    if (!kid.jumping && !kid.landing)kid.walking = true;
  }
  if (buttons.pressed(UP_BUTTON) && (kid.y > GAME_TOP))
  {

  }
  if (buttons.pressed(RIGHT_BUTTON) && (kid.x < GAME_RIGHT))
  {
    kid.direction = FACING_RIGHT;
    kid.x++;
    if (!kid.jumping && !kid.landing)kid.walking = true;
  }

  if (buttons.justPressed(A_BUTTON)) gameState = STATE_GAME_PAUSE;
  if (buttons.justPressed(B_BUTTON))
  {
    kid.walking = false;
    kid.jumping = true;
  }
}

#endif
