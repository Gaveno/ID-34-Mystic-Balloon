#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "globals.h"
#include "player.h"

void checkInputs()
{
kid.isWalking = false;
  if (arduboy.pressed(DOWN_BUTTON))
  {
    cam.offset.y = -CAMERA_OFFSET;
  }
  if (arduboy.pressed(LEFT_BUTTON))
  {
    cam.offset.x = CAMERA_OFFSET;
    kid.direction = FACING_LEFT;
    if (kid.speed.y == 0)
    {
      if (!gridGetSolid((kid.pos.x - 1) >> 4, (kid.pos.y + 8) >> 4))
        kid.actualpos.x -= PLAYER_SPEED_WALKING;
      kid.isWalking = true;
    }
    else
    {
      kid.speed.x = (kid.speed.x > -MAX_XSPEED) ? kid.speed.x - PLAYER_SPEED_AIR : kid.speed.x = -MAX_XSPEED;
    }
    //if (!kid.isJumping && !kid.isLanding)kid.isWalking = true;
  }
  if (arduboy.pressed(UP_BUTTON))
  {
    cam.offset.y = CAMERA_OFFSET;
  }
  if (arduboy.pressed(RIGHT_BUTTON))
  {
    cam.offset.x = -CAMERA_OFFSET;
    kid.direction = FACING_RIGHT;
    if (kid.speed.y == 0)
    {
      if (!gridGetSolid((kid.pos.x + 12) >> 4, (kid.pos.y + 8) >> 4))
        kid.actualpos.x += PLAYER_SPEED_WALKING;
      kid.isWalking = true;
    }
    else
    {
      kid.speed.x = (kid.speed.x < MAX_XSPEED) ? kid.speed.x + PLAYER_SPEED_AIR : kid.speed.x = MAX_XSPEED;
    }
    //if (!kid.isJumping && !kid.isLanding)kid.isWalking = true;
  }

  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PAUSE;
  if (arduboy.justPressed(B_BUTTON))
  {
    if (kid.speed.y == 0)
    {
      kid.isWalking = false;
      kid.isJumping = true;
      kid.jumpLetGo = false;
      kid.jumpTimer = 12;
      kid.speed.y = PLAYER_JUMP_VELOCITY;
      if (arduboy.pressed(RIGHT_BUTTON)) kid.speed.x = (PLAYER_SPEED_WALKING);
      if (arduboy.pressed(LEFT_BUTTON)) kid.speed.x = -(PLAYER_SPEED_WALKING);
    }
  }
  if (kid.isJumping && !arduboy.pressed(B_BUTTON))
  {
    kid.jumpLetGo = true;
  }
}

#endif
