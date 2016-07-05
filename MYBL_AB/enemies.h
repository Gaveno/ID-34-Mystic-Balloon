#ifndef ENEMIES_H
#define ENEMIES_H

#include <Arduino.h>
#include "globals.h"
//#include "vec2.h"

#define MAX_FAN_PARTICLES 4
#define FAN_POWER 5

#define SPIKES_LEFT         0
#define SPIKES_DOWN         1
#define SPIKES_RIGHT        2
#define SPIKES_UP           3


struct Coin
{
  vec2 pos;
  bool active;
};

Coin coins[MAX_PER_TYPE];

struct Key
{
  vec2 pos;
  bool active;
  bool haveKey;
};

Key key = {.pos = vec2(0, 0), .active = false, .haveKey = false};

struct Walker
{
  vec2 pos;
  int8_t direction;
  int8_t HP;
  bool hurt;
  bool active;
};

Walker walkers[MAX_PER_TYPE];

struct Spike
{
  Rect pos;
  byte characteristics;//B00000000;   //this byte holds all the enemies characteristics
  //                      ||||||||
  //                      |||||||└->  0 \ these 2 bits are used to determine the spike type
  //                      ||||||└-->  1 /  
  //                      |||||└--->  2   the spike is active    (0 = false / 1 = true)

  //bool active;
  //byte type;
};

  Spike spikes[MAX_PER_TYPE];

struct Fan
{
  vec2 pos;
  vec2 particles[5];
  int height;
  bool active;
};

Fan fans[MAX_PER_TYPE];

void enemiesInit()
{
  coinsActive = 0;
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    // Fans
    fans[i].pos = vec2(0, 0);
    for (byte a = 0; a < MAX_FAN_PARTICLES; ++a)
      fans[i].particles[a] = vec2(random(16), random(16));
    fans[i].height = 0;
    fans[i].active = false;

    // Spikes
    spikes[i].pos.x = 0;
    spikes[i].pos.y = 0;
    spikes[i].pos.width = 16;
    spikes[i].pos.height = 16;
    
    spikes[i].characteristics = 1;
    
    // Walkers
    walkers[i].pos.x = 0;
    walkers[i].pos.y = 0;
    walkers[i].active = false;
    walkers[i].HP = 40;
    walkers[i].direction = 1;
    walkers[i].hurt = false;

    // Coins
    coins[i].pos.x = 0;
    coins[i].pos.y = 0;
    coins[i].active = false;
  }
}

void coinsCreate(vec2 pos)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (!coins[i].active)
    {
      ++coinsActive;
      coins[i].pos = pos << 4;
      coins[i].pos.x += 2;
      coins[i].active = true;
      return;
    }
  }
}

void keyCreate(vec2 pos)
{
  key.pos = pos << 4;
  key.active = true;
  key.haveKey = false;
}

void walkersCreate(vec2 pos)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (!walkers[i].active)
    {
      walkers[i].pos = pos << 4;
      walkers[i].pos.y += 8;
      walkers[i].active = true;
      return;
    }
  }
}

void spikesCreate(vec2 pos)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (!bitRead(spikes[i].characteristics, 2))
    {
      
      spikes[i].pos.x = pos.x << 4;
      spikes[i].pos.y = pos.y << 4;
      // Solid above
      if (gridGetSolid(pos.x, pos.y - 1))
      {
        spikes[i].characteristics = B00000111;
        spikes[i].pos.width = 16;
        spikes[i].pos.height = 8;
      }
      // Solid below
      else if (gridGetSolid(pos.x, pos.y + 1))
      {
        spikes[i].characteristics = B00000101;
        spikes[i].pos.width = 16;
        spikes[i].pos.height = 8;
        spikes[i].pos.y += 8;
      }
      // Solid left
      else if (gridGetSolid(pos.x - 1, pos.y))
      {
        spikes[i].characteristics = B00000100;
        spikes[i].pos.width = 8;
        spikes[i].pos.height = 16;
      }
      // Solid right
      else if (gridGetSolid(pos.x + 1, pos.y))
      {
        spikes[i].characteristics = B00000110;
        spikes[i].pos.width = 8;
        spikes[i].pos.height = 16;
        spikes[i].pos.x += 8;
      }
      //bitSet(spikes[i].characteristics, 2);
      return;
    }
  }
}

void fansCreate(vec2 pos, byte height)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (!fans[i].active)
    {
      fans[i].pos = pos << 4;
      fans[i].height = height << 4;
      fans[i].active = true;
      return;
    }
  }
}

void enemiesUpdate()
{
  if (arduboy.everyXFrames(8))
  {
    walkerFrame = (++walkerFrame) % 2;
    coinFrame = (++coinFrame) % 4;
  }

  if (key.active)
  {
    int commonx = key.pos.x - cam.pos.x;
    int commony = key.pos.y - cam.pos.y;
    sprites.drawOverwrite(commonx, commony, elements, 4);
  }

  if (arduboy.everyXFrames(5)) fanFrame = (++fanFrame) % 3;
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    // Fans
    if (fans[i].active)
    {
      // Update
      if (arduboy.everyXFrames(2))
        for (byte a = 0; a < MAX_FAN_PARTICLES; ++a)
        {
          // Update Particles
          fans[i].particles[a].y =
            (fans[i].particles[a].y < (fans[i].height)) ?
            fans[i].particles[a].y + 6 : random(fans[i].height >> 2);

          // Draw particles
          sprites.drawErase(fans[i].pos.x + fans[i].particles[a].x - cam.pos.x, fans[i].pos.y - fans[i].particles[a].y - cam.pos.y, particle , 0);
        }

      // Draw fan
      int _x = fans[i].pos.x - cam.pos.x;
      int _y = fans[i].pos.y - cam.pos.y;
      sprites.drawOverwrite(_x, _y, fan, fanFrame);
    }

    // Spikes
    if (bitRead(spikes[i].characteristics, 2))
    {
      int commonx = spikes[i].pos.x - cam.pos.x;
      int commony = spikes[i].pos.y - cam.pos.y;
      sprites.drawOverwrite(commonx, commony, sprSpikes,  spikes[i].characteristics & B00000011);
      if (!bitRead(spikes[i].characteristics, 0)) sprites.drawOverwrite(commonx, commony + 8, sprSpikes,  spikes[i].characteristics & B00000011);
      else sprites.drawOverwrite(commonx + 8, commony, sprSpikes,  spikes[i].characteristics & B00000011);
    }

    // Walkers
    if (walkers[i].active)
    {
      if (arduboy.everyXFrames(2) && walkers[i].HP > 0 && !walkers[i].hurt)
      {
        if (!gridGetSolid((walkers[i].pos.x + 4 + (walkers[i].direction * 5)) >> 4, walkers[i].pos.y >> 4)
            && gridGetSolid((walkers[i].pos.x + 4 + (walkers[i].direction * 5)) >> 4, (walkers[i].pos.y >> 4) + 1))
        {
          walkers[i].pos.x += walkers[i].direction;
        }
        else
        {
          walkers[i].direction = -walkers[i].direction;
        }
      }

      sprites.drawOverwrite(walkers[i].pos.x - cam.pos.x, walkers[i].pos.y - cam.pos.y, walkerSprite, walkerFrame + (walkers[i].HP <= 0) * 2);
    }

    // Coins
    if (coins[i].active)
    {
      sprites.drawOverwrite(coins[i].pos.x - cam.pos.x, coins[i].pos.y - cam.pos.y, elements, coinFrame);
    }
  }
}


#endif
