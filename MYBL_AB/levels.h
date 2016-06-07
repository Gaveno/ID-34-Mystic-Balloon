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
#define LEVEL_CELL_BYTES (LEVEL_WIDTH_CELLS * LEVEL_HEIGHT_CELLS) >> 3
#define LEVEL_ARRAY_SIZE 576


//char gameGrid[LEVEL_ARRAY_SIZE]; // grid with cell information
// upper byte tile xxxx ____
// LSB solid ____ ___x

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

const uint8_t levelTest [] PROGMEM = {
// Tiles
0x31, 0x00, 0x00, 0x31, 0x00, 0x00, 0x39, 0x00, 0x00, 
0x0F, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// Objects

// EoL
0xFF
};

const uint8_t * levels[] = {
  0x00, level1,
};


// Cell based grid checking
bool gridGetSolid(int8_t x, int8_t y) {
  if (x < 0 || x >= LEVEL_WIDTH_CELLS || y >= LEVEL_HEIGHT_CELLS)
    return 1;

  if (y < 0)
    return 0;

  const uint8_t *lvl = levels[level];
  byte b = pgm_read_byte(lvl + (x >> 3) + (y * (LEVEL_WIDTH_CELLS >> 3)));
  return ((b >> (x % 8)) & 0x01);
}

byte gridGetTile(int8_t x, int8_t y) {
  if (!gridGetSolid(x, y)) return 0;
  //if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT || !gridGetSolid(x, y))
    //return 0;
  //return gameGrid[x + (y * LEVEL_WIDTH_CELLS)] >> 4;
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

  return i;
}


void levelLoad() {
  // TO-DO Objects
}

void drawGrid() {
  //Serial.println("Start of tile drawing");
  for ( int x = (cam.pos.x >> 4); x <= (cam.pos.x >> 4) + 8; ++x)
  {
    for ( int y = (cam.pos.y >> 4); y <= (cam.pos.y >> 4) + 4; ++y) {
      //if (x >= 0 && x < LEVEL_WIDTH
      //&& y >= 0 && y < LEVEL_HEIGHT)
      {
        /*int index = (i.y * LEVEL_WIDTH_CELLS) + i.x;
        i = i << 4;
        i -= cam.pos >> FIXED_POINT;*/
//        Serial.print("Pos: ");
//        Serial.print(x);
//        Serial.print(", ");
//        Serial.print(y);
//        Serial.print(" Tile: ");
//        Serial.print(gridGetTile(x, y));
//        Serial.print("\n");
        sprites.drawErase((x << 4) - cam.pos.x, (y << 4) - cam.pos.y, sprTileset, gridGetTile(x, y));
      }
    }
  }
  //Serial.println("End of tile drawing");
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
