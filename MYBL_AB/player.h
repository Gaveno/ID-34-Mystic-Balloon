#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "globals.h"
//#include "levels.h"
#include "vec2.h"

#define PLAYER_SPEED_WALKING 16
#define PLAYER_SPEED_AIR 2
#define PLAYER_JUMP_VELOCITY 32
#define GRAVITY 2
#define FRICTION 1 // for horizontal speed
#define FIXED_POINT 4
#define MAX_XSPEED PLAYER_SPEED_WALKING
#define MAX_YSPEED 3 * (1 << FIXED_POINT)
#define CAMERA_OFFSET 5

extern bool gridGetSolid(int8_t x, int8_t y);

struct Camera
{
  // x and y are 9.6 signed fixed vec2 values
  vec2 pos;
  vec2 offset;
};

struct Players
{
  public:
    // x and y are 9.6 signed fixed vec2 values
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
    byte balloons;
    byte balloonOffset;
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
  kid.balloons = 2;
  kid.balloonOffset = 0;
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
  if (kid.speed.y > 0 && !kid.isBalloon)
  {
    kid.isJumping = true;
    kid.isLanding = false;
    if (!kid.jumpLetGo && kid.jumpTimer > 0)
    {
      kid.speed.y += GRAVITY;
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
  boolean solidbelow = gridGetSolid((kid.pos.x + 10) >> 4, (kid.pos.y + 16) >> 4)
  | gridGetSolid((kid.pos.x + 2) >> 4, (kid.pos.y + 16) >> 4);
  boolean solidabove = gridGetSolid((kid.pos.x + 10) >> 4, (kid.pos.y - 1) >> 4)
  | gridGetSolid((kid.pos.x + 2) >> 4, (kid.pos.y - 1) >> 4);
  boolean solidleft = gridGetSolid((kid.pos.x - 1) >> 4, (kid.pos.y + 15) >> 4)
  | gridGetSolid((kid.pos.x - 1) >> 4, (kid.pos.y + 1) >> 4);
  boolean solidright = gridGetSolid((kid.pos.x + 13) >> 4, (kid.pos.y + 15) >> 4)
  | gridGetSolid((kid.pos.x + 13) >> 4, (kid.pos.y + 1) >> 4);
  boolean solidH = gridGetSolid(
    (((kid.actualpos.x + kid.speed.x) >> FIXED_POINT) - 1 + (kid.speed.x > 0) * 14) >> 4,
    (((kid.actualpos.y) >> FIXED_POINT) + 8) >> 4
  );
  boolean solidV = gridGetSolid(
    (((kid.actualpos.x) >> FIXED_POINT) + 6) >> 4,
    (((kid.actualpos.y - kid.speed.y) >> FIXED_POINT) + (kid.speed.y < 0) * 17) >> 4
  );

  // Gravity
  if (kid.speed.y > 0 || !solidbelow)
  {
    //kid.speed.y += GRAVITY;
    kid.speed.y = (kid.speed.y > -MAX_YSPEED) ? kid.speed.y - GRAVITY : -MAX_YSPEED;
    if (kid.isBalloon)
    {
      if (kid.balloonOffset > 0)
        kid.balloonOffset -= 2;
      else
        kid.speed.y = max(-((10/kid.balloons) >> 1), kid.speed.y);
    }
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
  if (kid.speed.y <= 0 && (solidV || solidbelow))
  {
    kid.speed.y = 0;
    kid.speed.x = 0;
    kid.isLanding = false;
    kid.isJumping = false;
    kid.isBalloon = false;
    kid.actualpos.y = (((kid.actualpos.y >> FIXED_POINT) + 8) >> 4) << (FIXED_POINT + 4);

    if (!gridGetSolid((kid.pos.x + 2) >> 4, (kid.pos.y + 16) >> 4))
      kid.actualpos.x -= 8;
    if (!gridGetSolid((kid.pos.x + 10) >> 4, (kid.pos.y + 16) >> 4))
      kid.actualpos.x += 8;
  }

  // Move out of walls
  if (gridGetSolid((kid.pos.x + 1) >> 4, (kid.pos.y + 8) >> 4))
    kid.actualpos.x += 8;
  if (gridGetSolid((kid.pos.x + 11) >> 4, (kid.pos.y + 8) >> 4))
    kid.actualpos.x -= 8;
  
  // -Y Position
  //if ((kid.speed.y > 0 && !solidV)
  //|| (kid.speed.y < 0 && !solidV))
  if (!solidV && kid.speed.y != 0)
  {
    kid.actualpos.y -= kid.speed.y;
  }
  else
  {
    if (solidabove)
    {
      kid.actualpos.y = ((kid.actualpos.y + 2) >> (FIXED_POINT + 4)) << (FIXED_POINT + 4);
      kid.speed.y = 0;
    }
  }

  // -X Position
  //if ((kid.speed.x < 0 && !solidH) || (kid.speed.x > 0 && !solidH))
  if (kid.speed.x != 0)
  {
    if (!solidH)
    {
      kid.actualpos.x += kid.speed.x;
    }
    else
    {
      //if (kid.speed.x < 0)
      kid.speed.x = 0;
      kid.actualpos.x = (((((kid.actualpos.x >> FIXED_POINT) + 6) >> 4) << 4) + ((!kid.direction) * 4)) << (FIXED_POINT);
      //kid.actualpos.x += ((kid.speed.x > 0) * 4) << FIXED_vec2;
    }
  }

  kid.pos = (kid.actualpos >> FIXED_POINT);
}

void updateCamera()
{
  // Camera offset
  if (cam.offset.x > 0) cam.offset.x--;
  if (cam.offset.x < 0) cam.offset.x++;
  if (cam.offset.y > 0) cam.offset.y--;
  if (cam.offset.y < 0) cam.offset.y++;
  
  vec2 kp, cp;
  kp = kid.pos;
  cp = (cam.pos + cam.offset);

  vec2 V;
  //vec2 V = (kid.pos - cam.pos + cam.offset) >> 3; // more bytes
  V.x = kp.x - cp.x - 58;
  V.y = kp.y - cp.y - 24;
  V = V >> 3;

  cam.pos += V;
  //cam.pos.x = kid.pos.x - 64;
  //cam.pos.y = kid.pos.y - 32;
}

void drawKid()
{
  if (kid.isActive)
  {
    if (kid.isBalloon)
    {
      if (kid.balloons > 1) {
        sprites.drawPlusMask
            (
                kid.pos.x - cam.pos.x  + (1 * kid.direction), kid.pos.y - cam.pos.y - 13 + kid.balloonOffset, balloon_plus_mask, kid.direction
            );
      }
      sprites.drawPlusMask
        (
            kid.pos.x - cam.pos.x + 4 - (7 * kid.direction), kid.pos.y - cam.pos.y - 12 + kid.balloonOffset, balloon_plus_mask, kid.direction
        );
    }
    sprites.drawPlusMask
        (
            kid.pos.x - cam.pos.x,
              kid.pos.y - cam.pos.y, kidWalking_plus_mask,
              kid.frame + 6 * kid.direction + 4 * kid.isJumping + 5 * (kid.isLanding || kid.isBalloon)
        );
  }
}


#endif
