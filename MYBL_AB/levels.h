#ifndef LEVELS_H
#define LEVELS_H

#include <Arduino.h>
#include "globals.h"
#include "enemies.h"
#include "vec2.h"
#include "player.h"

#define LEVEL_WIDTH 384    // 24 * 16
#define LEVEL_HEIGHT 384   // 24 * 16
#define LEVEL_CELLSIZE 16
#define LEVEL_WIDTH_CELLS 24
#define LEVEL_HEIGHT_CELLS 24
#define LEVEL_ARRAY_SIZE 576


char gameGrid[LEVEL_ARRAY_SIZE]; // grid with cell information
// upper byte tile xxxx ____
// LSB solid ____ ___x

// Cell based grid checking
bool gridGetSolid(int16_t x, int16_t y) {
  if (x < 0 || x > LEVEL_WIDTH_CELLS || y < 0 || y > LEVEL_HEIGHT_CELLS)
    return true;

  return gameGrid[x + (y * LEVEL_WIDTH_CELLS)] & 0x01;
}

char gridGetTile(int16_t x, int16_t y) {
  if (x < 0 || x > LEVEL_WIDTH || y < 0 || y > LEVEL_HEIGHT)
    return 0;

  return gameGrid[x + (y * LEVEL_WIDTH_CELLS)] >> 4;
}

const uint8_t level1 [] PROGMEM = {
// Tiles
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0xE6, 
0x80, 0x63, 0xE6, 0xFF, 0x03, 0x00, 0xFF, 0x01, 0x00, 
0x80, 0x01, 0x00, 0x80, 0x01, 0x00, 0x80, 0x01, 0x00, 
0x80, 0x01, 0x00, 0x80, 0x01, 0x00, 0x80, 0x01, 0x00, 
0x80, 0xFF, 0xFD, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x07, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// Objects
68,1,35,13,50,10,9,0,1,21,6,
// EoL
0xFF
};

const uint8_t * levels[] = {
  level1
};


void levelLoad(byte level) {
  // Set Solid Cells
  for (byte i = 0; i < LEVEL_ARRAY_SIZE; ++i) {
    //pgm_read_byte(rm + i)
    gameGrid[i] = (pgm_read_byte(levels[level] + (i / 8)) >> (i % 8)) & 0x01;
  }

  // Set Tiles
  for (byte x = 0; x < LEVEL_WIDTH_CELLS; ++x) {
    for (byte y = 0; y < LEVEL_HEIGHT_CELLS; ++y) {
      byte l, r, t, b, f, i;
      l = gridGetSolid(x - 1, y);
      t = gridGetSolid(x, y - 1);
      r = gridGetSolid(x + 1, y);
      b = gridGetSolid(x, y + 1);

      f = 0;
      f |= t << 3;
      f |= l << 2;
      f |= r << 1;
      f |= b;

      switch (f) {
        case 3: i = 1; break;
        case 7: i = 2; break;
        case 5: i = 3; break;
        case 11: i = 4; break;
        case 15: i = 5; break; // solid all around
        case 13: i = 6; break;
        case 10: i = 7; break;
        case 14: i = 8; break;
        case 12: i = 9; break;
        default: i = 0;
      }

      gameGrid[x + (y * LEVEL_WIDTH_CELLS)] |= i << 4;
    }
  }

  // TO-DO Objects
}

void drawGrid() {
  vec2 i(cam.pos.x >> (FIXED_POINT + 4), cam.pos.y >> (FIXED_POINT + 4));
  for ( ; i.x <= i.x + 8; ++i.x)
  {
    for ( ; i.y <= i.y + 8; ++i.y) {
      if (i.x >= 0 && i.x < LEVEL_WIDTH_CELLS
      && i.y >= 0 && i.y < LEVEL_HEIGHT_CELLS)
      {
        int index = (i.y * LEVEL_WIDTH_CELLS) + i.x;
        i = i << 4;
        i -= cam.pos >> FIXED_POINT;
        sprites.drawErase(i.x, i.y, sprTileset, gameGrid[index] >> 4);
      }
    }
  }
}

void checkCollisions()
{
  /*
  Rect playerRect = {.x = player.x + 2, .y = player.y +2, .width = 12, .height = 12};
  Rect enemyRect = {.x = enemy[i].x, .y = enemy[i].y, .width = 16, .height = 16};
  if (physics.collide(playerRect, enemyRect)
  */
}
#endif
