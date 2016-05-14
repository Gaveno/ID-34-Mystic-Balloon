#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "globals.h"

struct Players
{
  public:
    int x;
    int y;
    boolean isActive;
    boolean isImune;
    boolean direction;
    boolean isWalking;
    boolean isJumping;
    boolean isLanding;
    boolean isFloating;
    byte imuneTimer;
    byte jumpTimer;
    byte HP;
    byte frame;
};

Players kid;

void setKid()
{
  kid.x = 0;
  kid.y = 40;
  kid.isActive = true;
  kid.HP = 4;
  kid.isImune = true;
  kid.imuneTimer = 0;
  kid.jumpTimer = 0;
  kid.direction = FACING_RIGHT;
  kid.isWalking = false;
  kid.isJumping = false;
  kid.isLanding = false;
  kid.isFloating = false;
}

void checkKid()
{
  if (kid.isImune)
  {
    if (arduboy.everyXFrames(3)) kid.isActive = !kid.isActive;
    kid.imuneTimer++;
    if (kid.imuneTimer > 60)
    {
      kid.imuneTimer = 0;
      kid.isImune = false;
      kid.isActive = true;
    }
  }
  if (kid.HP < 2) gameState = STATE_GAME_OVER;
  if (arduboy.everyXFrames(8) && kid.isWalking) kid.frame++;
  if (kid.frame > 3 || !kid.isWalking) kid.frame = 0;
  if (kid.isJumping && kid.jumpTimer < 20)
  {
    kid.jumpTimer++;
    kid.y--;
  }
  if (kid.jumpTimer > 19)
  {
    kid.isJumping = false;
    kid.isLanding = true;
    kid.jumpTimer++;
    kid.y++;
  }
  if (kid.jumpTimer > 39)
  {
    kid.isLanding = false;
    kid.jumpTimer = 0;

  }

}

void drawKid()
{
  if (kid.isActive)
  {
    sprites.drawPlusMask(kid.x  + (1 * kid.direction), kid.y - 13, balloon_plus_mask, kid.direction);
    sprites.drawPlusMask(kid.x + 4 - (7 * kid.direction), kid.y - 12, balloon_plus_mask, kid.direction);
    sprites.drawPlusMask(kid.x, kid.y, kidWalking_plus_mask, kid.frame + 6 * kid.direction + 4 * kid.isJumping + 5 * kid.isLanding);
  }
}


#endif
