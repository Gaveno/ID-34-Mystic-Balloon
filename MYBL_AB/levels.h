#ifndef LEVELS_H
#define LEVELS_H

#include <Arduino.h>
#include "globals.h"
#include "enemies.h"

const uint8_t level1 [] PROGMEM = {
// Tiles
0,0,0,
0,0,0,
0,0,0,
0,0,0,
0,0,0,
0,99,230,
128,99,230,
255,3,0,
255,1,0,
128,1,0,
128,1,0,
128,1,0,
128,1,0,
128,1,0,
128,1,0,
128,255,253,
0,255,255,
0,0,0,
0,0,0,
0,0,0,
0,0,0,
0,0,0,
0,0,0,
0,0,0,
// Objects
68,1,35,13,50,10,9,0,1,21,6,
};





void checkCollisions()
{
  /*
  Rect playerRect = {.x = player.x + 2, .y = player.y +2, .width = 12, .height = 12};
  Rect enemyRect = {.x = enemy[i].x, .y = enemy[i].y, .width = 16, .height = 16};
  if (physics.collide(playerRect, enemyRect)
  */
}
#endif
