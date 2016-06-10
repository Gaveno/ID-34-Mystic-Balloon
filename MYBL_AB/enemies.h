#ifndef ENEMIES_H
#define ENEMIES_H

#include <Arduino.h>
#include "globals.h"
//#include "vec2.h"

#define MAX_PER_TYPE 6 // total instances per enemy type

#define MAX_FAN_PARTICLES 4
#define FAN_POWER 3

struct ActiveObject
{
  vec2 pos;
  bool active;
};

struct Fan : public ActiveObject
{
  vec2 particles[5];
  byte height;
};

Fan fans[MAX_PER_TYPE];

void fansInit()
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    fans[i].pos = vec2(0, 0);
    for (byte a = 0; a < MAX_FAN_PARTICLES; ++a)
      fans[i].particles[a] = vec2(random(16), random(16));
    fans[i].height = 0;
    fans[i].active = false;
  }
}

void fansCreate(vec2 pos, byte height)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (!fans[i].active)
    {
      fans[i].pos = pos << 4;
      fans[i].height = height;
      fans[i].active = true;
      return;
    }
  }
}

void fansUpdate()
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (fans[i].active)
    {
      // Update
      if (arduboy.everyXFrames(2))
      for (byte a = 0; a < MAX_FAN_PARTICLES; ++a)
      {
        // Update Particles
        fans[i].particles[a].y =
          (fans[i].particles[a].y < (fans[i].height << 4)) ?
          fans[i].particles[a].y + 6 : random(fans[i].height << 2);
  
        // Draw particles
        arduboy.drawPixel(fans[i].pos.x + fans[i].particles[a].x - cam.pos.x,
                          fans[i].pos.y - fans[i].particles[a].y - cam.pos.y, 0);
      }
  
      // Draw fan
      sprites.drawErase(fans[i].pos.x - cam.pos.x, fans[i].pos.y - cam.pos.y, sprFan, millis() % 2);
    }
  }
}


#endif
