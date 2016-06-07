#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "globals.h"
//#include "levels.h"
#include "vec2.h"

#define PLAYER_SPEED_WALKING 8
#define PLAYER_SPEED_AIR 5
#define PLAYER_JUMP_VELOCITY 32
#define GRAVITY 2
#define FRICTION 1 // for horizontal speed
#define FIXED_POINT 4
#define MAX_XSPEED 2 * (1 << FIXED_POINT)
#define MAX_YSPEED 3 * (1 << FIXED_POINT)

extern bool gridGetSolid(int8_t x, int8_t y);

struct Camera
{
  // x and y are 9.6 signed fixed point values
  vec2 pos;
  vec2 offset;
};

struct Players
{
  public:
    // x and y are 9.6 signed fixed point values
    vec2 pos;
    vec2 actualpos;
    vec2 speed;
    boolean isActive;
    boolean isImune;
    boolean direction;
    boolean isWalking;
    boolean isJumping;
    boolean isLanding;
    boolean isFloating;
    boolean isBalloon;
    boolean jumpLetGo;
    byte imuneTimer;
    byte jumpTimer;
    byte HP;
    byte frame;
};

Players kid;
Camera cam;

void setKid()
{
  kid.pos.x = 0;
  kid.pos.y = 0;
  kid.actualpos.x = 128;
  kid.actualpos.y = 0;
  kid.speed.x = 0;
  kid.speed.y = 0;
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
  kid.isBalloon = false;
  kid.jumpLetGo = true;
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

  // Kid is moving up
  if (kid.speed.y > 0)
  {
    kid.isJumping = true;
    if (!kid.jumpLetGo && kid.jumpTimer > 0)
    {
      kid.speed.y -= GRAVITY << 1;
      kid.jumpTimer--;
    }
  }

  // Kid is moving down
  if (kid.speed.y < 0)
  {
    kid.isJumping = false;
    kid.isLanding = true;
  }

  // Update position---
  // -Solid checking
  boolean solidbelow = gridGetSolid((kid.pos.x + 6) >> 4, (kid.pos.y + 16 + min(0, kid.speed.y >> FIXED_POINT)) >> 4);
  boolean solidabove = gridGetSolid((kid.pos.x + 6) >> 4, (kid.pos.y - 1 - max(0, kid.speed.y >> FIXED_POINT)) >> 4);
  boolean solidleft = gridGetSolid((kid.pos.x - min(kid.speed.x >> FIXED_POINT, 0) - 1) >> 4, (kid.pos.y + 8) >> 4);
  boolean solidright = gridGetSolid((kid.pos.x + max(kid.speed.x >> FIXED_POINT, 0) + 13) >> 4, (kid.pos.y + 8) >> 4);

  // Gravity
  if (kid.speed.y > 0 || !solidbelow)
  {
    //kid.speed.y += GRAVITY;
    kid.speed.y = (kid.speed.y > -MAX_YSPEED) ? kid.speed.y - GRAVITY : -MAX_YSPEED;
  }

  // Friction
  if (abs(kid.speed.x) > FRICTION) 
  {
    if (kid.speed.x > 0) kid.speed.x -= FRICTION;
    if (kid.speed.x < 0) kid.speed.x += FRICTION;
  }
  else
  {
    kid.speed.x = 0;
  }

  // Kid on ground
  if (kid.speed.y <= 0 && solidbelow)
  {
    kid.speed.y = 0;
    kid.isLanding = false;
    kid.isJumping = false;
  }
  
  // -Y Position
  if ((kid.speed.y > 0 && !solidabove)
  || (kid.speed.y < 0 && !solidbelow))
  {
    kid.actualpos.y -= kid.speed.y;
  }

  // -X Position
  if ((kid.speed.x < 0 && !solidleft) || (kid.speed.x > 0 && !solidright))
  {
    kid.actualpos.x += kid.speed.x;
  }

  kid.pos = (kid.actualpos >> FIXED_POINT);
  /*if (kid.isJumping && kid.speed.y <= 0 //kid.jumpTimer < 20)
  {
    kid.jumpTimer++;
    kid.pos.y += kid.speed.y;
  }
  if (kid.jumpTimer > 19)
  {
    kid.isJumping = false;
    kid.isLanding = true;
    kid.jumpTimer++;
    kid.pos.y += 64;
  }
  if (kid.jumpTimer > 39)
  {
    kid.isLanding = false;
    kid.jumpTimer = 0;

  }*/
}

void updateCamera()
{
  /*vec2 kp, cp;
  kp = kid.pos >> FIXED_POINT;
  cp = (cam.pos + cam.offset);

  vec2 V;
  //V.x = kp.x - cp.x - 58;
  //V.y = kp.y - cp.y - 24;
  V = kp - vec2(58, 24);
  //V = V >> 2;

  cam.pos += V;
  //cam.pos.x = kid.pos.x - 64;
  //cam.pos.y = kid.pos.y - 32;*/
}

void drawKid()
{
  if (kid.isActive)
  {
    if (kid.isBalloon)
    {
      sprites.drawPlusMask
          (
              kid.pos.x - cam.pos.x  + (1 * kid.direction), kid.pos.x - cam.pos.y - 13, balloon_plus_mask, kid.direction
          );
      sprites.drawPlusMask
          (
              kid.pos.x - cam.pos.x + 4 - (7 * kid.direction), kid.pos.x - cam.pos.y - 12, balloon_plus_mask, kid.direction
          );
    }
    sprites.drawPlusMask
        (
            kid.pos.x - cam.pos.x,
              kid.pos.y - cam.pos.y, kidWalking_plus_mask,
              kid.frame + 6 * kid.direction + 4 * kid.isJumping + 5 * kid.isLanding
        );
  }
}


#endif
