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
    boolean walking;
    boolean jumping;
    boolean landing;
    byte imuneTimer;
    byte jumpTimer;
    byte HP;
    byte frame;
};

Players kid;

void setKid()
{
  kid.x = 0;
  kid.y = 20;
  kid.isActive = true;
  kid.HP = 4;
  kid.isImune = true;
  kid.imuneTimer = 0;
  kid.jumpTimer = 0;
  kid.direction = FACING_RIGHT;
  kid.walking = false;
  kid.jumping = false;
  kid.landing = false;
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
  if (arduboy.everyXFrames(8) && kid.walking) kid.frame++;
  if (kid.frame > 3 || !kid.walking) kid.frame = 0;
  if (kid.jumping && kid.jumpTimer < 20)
  {
   kid.jumpTimer++;
   kid.y--;
  }
  if(kid.jumpTimer > 19)
  {
    kid.jumping = false;
    kid.landing = true;
    kid.jumpTimer++;
    kid.y++;
  }
  if(kid.jumpTimer > 39)
  {
    kid.landing = false;
    kid.jumpTimer = 0;
    
  }
  
}

void drawKid()
{
  if (kid.isActive) sprites.drawPlusMask(kid.x, kid.y, kidWalking_plus_mask, kid.frame + 6 * kid.direction + 4*kid.jumping + 5*kid.landing);
}


#endif
