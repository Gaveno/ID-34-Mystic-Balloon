#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "globals.h"
#include "player.h"

#define TIMER_AMOUNT 48

void checkInputs()
{
  cam.offset = vec2(0, 0);
  kid.isWalking = false;
  if (arduboy.pressed(DOWN_BUTTON))
  {
    cam.offset.y = CAMERA_OFFSET;
  }
  if (arduboy.pressed(LEFT_BUTTON) && !kid.isSucking)
  {
    mapTimer = TIMER_AMOUNT;
    cam.offset.x = -CAMERA_OFFSET;
    kid.direction = FACING_LEFT;
    if (kid.speed.y == 0 && !kid.isBalloon)
    {
      if (!gridGetSolid((kid.pos.x - 1) >> 4, (kid.pos.y + 8) >> 4))
        kid.actualpos.x -= PLAYER_SPEED_WALKING;
      kid.isWalking = true;
    }
    else //if (arduboy.everyXFrames(3))
    {
      kid.speed.x = (kid.speed.x > -MAX_XSPEED) ? kid.speed.x - PLAYER_SPEED_AIR : kid.speed.x = -MAX_XSPEED;
    }
    //if (!kid.isJumping && !kid.isLanding)kid.isWalking = true;
  }
  if (arduboy.pressed(UP_BUTTON))
  {
    cam.offset.y = -CAMERA_OFFSET;
  }
  if (arduboy.pressed(RIGHT_BUTTON) && !kid.isSucking)
  {
    //mapTimer = TIMER_AMOUNT;
    cam.offset.x = CAMERA_OFFSET;
    kid.direction = FACING_RIGHT;
    if (kid.speed.y == 0 && !kid.isBalloon)
    {
      if (!gridGetSolid((kid.pos.x + 12) >> 4, (kid.pos.y + 8) >> 4))
        kid.actualpos.x += PLAYER_SPEED_WALKING;
      kid.isWalking = true;
    }
    else //if (arduboy.everyXFrames(3))
    {
      kid.speed.x = (kid.speed.x < MAX_XSPEED) ? kid.speed.x + PLAYER_SPEED_AIR : kid.speed.x = MAX_XSPEED;
    }
    //if (!kid.isJumping && !kid.isLanding)kid.isWalking = true;
  }

  //if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PAUSE;
  if (arduboy.pressed(A_BUTTON) && !kid.isBalloon)
  {
    kid.isSucking = true;
    //mapTimer = TIMER_AMOUNT;
  }
  else
    kid.isSucking = false;
  if (arduboy.justPressed(B_BUTTON))
  {
    //mapTimer = TIMER_AMOUNT;
    if (kid.speed.y == 0 && kid.isJumping == false && kid.isLanding == false)
    {
      arduboy.audio.tone(200, 100);
      kid.isWalking = false;
      kid.isJumping = true;
      kid.jumpLetGo = false;
      kid.jumpTimer = 12;
      kid.speed.y = PLAYER_JUMP_VELOCITY;
      if (arduboy.pressed(RIGHT_BUTTON)) kid.speed.x = (PLAYER_SPEED_WALKING);
      if (arduboy.pressed(LEFT_BUTTON)) kid.speed.x = -(PLAYER_SPEED_WALKING);
    }
    else
    {
      if (kid.balloons > 0)
      {
        kid.isBalloon = true;
        kid.balloonOffset = 16;
        kid.isJumping = false;
        kid.isLanding = true;
        //kid.speed.y = GRAVITY;
      }
    }
  }
  if (!arduboy.pressed(B_BUTTON))
  {
    kid.isBalloon = false;
    if (kid.isJumping) kid.jumpLetGo = true;
  }
}

#endif
