#ifndef ENEMIES_H
#define ENEMIES_H

#include <Arduino.h>
#include "globals.h"
#include "vec2.h"

#define MAX_FANS 5
#define MAX_FAN_PARTICLES 5

struct Fan
{
  vec2 pos;
  vec2 particles[5];
  byte height;
  bool active;
};

Fan fans[MAX_FANS];

void fansInit()
{
  for (byte i = 0; i < MAX_FANS; ++i)
  {
    fans[i].pos = vec2(0, 0);
    for (byte a = 0; a < MAX_FAN_PARTICLES; ++a)
      fans[i].particles[a] = fans[i].pos + vec2(random(16), random(16));
    fans[i].height = 0;
    fans[i].active = false;
  }
}

void fansCreate(vec2 pos, byte height)
{
  for (byte i = 0; i < MAX_FANS; ++i)
  {
    if (!fans[i].active)
    {
      fans[i].pos = pos;
      fans[i].height = height;
      fans[i].active = true;
      return;
    }
  }
}

void fansUpdate()
{
  for (byte i = 0; i < MAX_FANS; ++i)
  {
    // Update
    if (arduboy.everyXFrames(2))
    for (byte a = 0; a < MAX_FAN_PARTICLES; ++a)
    {
      // Update Particles
      fans[i].particles[a].y =
        (fans[i].particles[a].y > fans[i].pos.y - (fans[i].height << 4)) ?
        fans[i].particles[a].y - 1 : fans[i].pos.y;

      // Draw particles
      arduboy.drawPixel(fans[i].particles[a].x, fans[i].particles[a].y, 0);
    }

    // Draw fan
    // TO-DO
  }
}


#endif
