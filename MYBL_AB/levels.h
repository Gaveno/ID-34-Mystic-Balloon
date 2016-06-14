#ifndef LEVELS_H
#define LEVELS_H

#include <Arduino.h>
#include "globals.h"
#include "enemies.h"
//#include "Point.h"
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
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 
0x0F, 0x80, 0x0F, 0x0F, 0x00, 0x1C, 0x0F, 0x00, 0xC0, 
0x00, 0x00, 0xC0, 0x00, 0x00, 0xF0, 0x00, 0x00, 0xF0, 
0x60, 0x55, 0xF5, 0x68, 0x00, 0x00, 0x01, 0x00, 0x00, 
0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0F, 0xFC, 0x01, 
0x1F, 0xFE, 0x0F, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x10, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x60, 0xC0, 
0x80, 0x61, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
// Objects
0x05,0x41,0x30,0xCE,0x03,0x30,0xCB,0x03,0x30,0xC8,0x03,
0x30,0xC5,0x03,0x10,0x81,
// EoL
0xFF
};

const uint8_t level2 [] PROGMEM = {
// Tiles
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x0F, 0x7F, 0x06, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x39, 
0x00, 0x7C, 0x30, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0xC0, 0x00, 0x7F, 0xFE, 0xFC, 0xFF, 0xFF, 
0xFC, 0x80, 0x01, 0x0C, 0x00, 0x00, 0x0C, 0x00, 0x00, 
0x0C, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x0C, 0x00, 0xF0, 
0x0C, 0x00, 0xF0, 0x0C, 0x00, 0xF0, 0x00, 0x00, 0xF0, 
0x00, 0x00, 0xF0, 0x3F, 0x00, 0xF0, 0x3F, 0x00, 0xF0, 
// Objects
0x35,0xD0,0x17,0x35,0xCF,0x17,0x35,0xC6,0x0A,0x14,0x16
,0x43,0x46,0x42,0x90,0x42,0x8F,0x22,0x4D,0x22,0x49,0x40,
0xD0,0x40,0xCF,0x20,0xC5,0x00,0x81,
// EoL
0xFF
};

const uint8_t level4 [] PROGMEM = {
// Tiles
0xC0, 0xC1, 0xFF, 0xC0, 0xCD, 0xFF, 0xC0, 0xCD, 0x00, 
0xD7, 0xCD, 0x00, 0xC7, 0xCD, 0x00, 0xC0, 0xCD, 0x03, 
0xFC, 0xDD, 0x03, 0x7C, 0xCC, 0x0F, 0x0C, 0xCC, 0x8F, 
0x0C, 0xCC, 0x80, 0x0C, 0xCC, 0xC0, 0x8C, 0xCF, 0xC0, 
0x8C, 0xCF, 0xF0, 0x8C, 0xEF, 0xF1, 0x8C, 0xCF, 0x01, 
0x8C, 0xCF, 0x03, 0x8C, 0xCF, 0x03, 0x8C, 0xCF, 0x0F, 
0x8C, 0xCF, 0x8F, 0x8C, 0x0F, 0x80, 0x80, 0x0F, 0xC0, 
0x80, 0x0F, 0xC0, 0xBF, 0xFF, 0xF3, 0xFF, 0xFF, 0xF3, 
// Objects
0x35,0x86,0x06,0x45,0x4C,0x34,0x44,0x08,0x24,0x12,0x43,
0xC6,0x43,0x0D,0x22,0xD5,0x32,0x89,0x09,0x42,0x04,0x21,
0x92,0x41,0x4C,0x41,0x40,0x11,0x10,0x00,0x81,
// EoL
0xFF
};

const uint8_t levelTest [] PROGMEM = {
// Tiles
0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0xC0, 0x60, 0x1C, 
0xC7, 0x67, 0x1C, 0x7F, 0x07, 0x30, 0x38, 0x06, 0x30, 
0x00, 0xFC, 0x33, 0x00, 0xF8, 0x33, 0xC0, 0x00, 0x30, 
0xC0, 0x00, 0x30, 0x00, 0xFC, 0x3F, 0x0C, 0xFC, 0x0F, 
0x0C, 0x06, 0x00, 0x30, 0x07, 0xC0, 0x30, 0x03, 0xFE, 
0xF0, 0x01, 0x3E, 0xF1, 0xB8, 0x03, 0x33, 0xB8, 0x01, 
0x33, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0xC0, 
0x83, 0xC1, 0xC0, 0xFF, 0xC1, 0xFF, 0xFF, 0xC1, 0xFF, 
// Objects
0x35,0xC9,0x08,0x15,0x54,0x43,0x87,0x43,0x86,0x42,0x97,0x22,0x4E,0x21,0x4F,0x21,0x4C,0x41,0x17,0x00,0x81,
// EoL
0xFF
};

const uint8_t * levels[] = {
  level1, level2, levelTest, level4,
};


// Cell based grid checking
bool gridGetSolid(int8_t x, int8_t y) {
  if (x < 0 || x >= LEVEL_WIDTH_CELLS)
    return 1;

  if (y < 0 || y >= LEVEL_HEIGHT_CELLS)
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
    default: i = 10;
  }

  return i;
}


void levelLoad(const uint8_t *lvl) {
  byte i = 0;
  lvl += LEVEL_ARRAY_SIZE >> 3;
  
  byte b = pgm_read_byte(lvl);
  while (b != 0xFF)
  {
    byte id, x, y, h;
    id = pgm_read_byte(lvl + i) >> 4;
    y = ((pgm_read_byte(lvl + i) & 0x0F) << 2) | (pgm_read_byte(lvl + (++i)) >> 6);
    x = pgm_read_byte(lvl + i++) & 0x3F;
    switch ( id )
    {
      case 0:
      {
        // Start
        startPos.x = (int)x << (FIXED_POINT + 4);
        startPos.y = (int)y << (FIXED_POINT + 4);
        //kid.actualpos.x = (int)x << (FIXED_POINT + 4);
        //kid.actualpos.y = (int)y << (FIXED_POINT + 4);
        kid.actualpos = startPos;
      }
      break;
      case 1:
      {
        // Finish
        levelExit.x = x << 4;
        levelExit.y = y << 4;
      }
      break;
      case 2:
      {
        // Walker
        walkersCreate(vec2(x, y));
      }
      break;
      case 3:
      {
        // Fan
        fansCreate(vec2(x, y), pgm_read_byte(lvl + i++));
      }
      break;
      case 4:
      {
        // Spikes
        spikesCreate(vec2(x, y));
      }
      break;
      default:
      break;
    }

    b = pgm_read_byte(lvl + i);
  }
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

  sprites.drawPlusMask(levelExit.x - cam.pos.x, levelExit.y - cam.pos.y, sprExit, walkerFrame);
  //Serial.println("End of tile drawing");
}

void checkCollisions()
{
  // Fall off earth
  if (kid.pos.y > LEVEL_HEIGHT)
  {
    if (kid.balloons == 1)
    {
      // dead
      gameState = STATE_GAME_OVER;
    }
    else
    {
      arduboy.audio.tone(300, 100);
      kid.actualpos = startPos;
      kid.balloons--;
      kid.isImune = true;
      kid.imuneTimer = 0;
    }
  }
  // Level exit
  Rect playerRect = {.x = kid.pos.x + 2, .y = kid.pos.y + 2, .width = 8, .height = 12};
  Rect playerSuckRect = {.x = kid.pos.x + ((kid.direction ^ 1) * 16) - (kid.direction * 16), .y = kid.pos.y + 2, .width = 16, .height = 14};
      
  if (arduboy.collide(levelExit, playerRect))
  {
    gameState = STATE_GAME_NEXT_LEVEL;
  }
  
  // Enemies and objects
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    // Walkers
    // Getting Sucked In
    if (walkers[i].active)
    {
      Rect walkerrect = {.x = walkers[i].pos.x, .y = walkers[i].pos.y, .width = 8, .height = 8};
      if (kid.isSucking && arduboy.collide(playerSuckRect, walkerrect))
      {
        --walkers[i].HP;
        walkers[i].hurt = true;
        if (walkers[i].HP <= 0)
        {
          if (kid.direction)
          {
            ++walkers[i].pos.x;
            if (walkers[i].pos.x > kid.pos.x - 8)
            {
              walkers[i].active = false;
              if (kid.balloons < 2) ++kid.balloons;
              else scorePlayer += 100;
              scorePlayer += 50;
              arduboy.audio.tone(200, 100);
            }
          }
          else
          {
            --walkers[i].pos.x;
            if (walkers[i].pos.x < kid.pos.x + 16)
            {
              walkers[i].active = false;
              if (kid.balloons < 3) ++kid.balloons;
              else scorePlayer += 100;
              scorePlayer += 50;
              arduboy.audio.tone(200, 100);
            }
          }
        }
      }
      else
        walkers[i].hurt = false;

      // Hurt player
      if (walkers[i].HP > 0 && !kid.isImune && arduboy.collide(playerRect, walkerrect))
      {
        if (kid.balloons == 1)
        {
          // dead
          gameState = STATE_GAME_OVER;
        }
        else
        {
          kid.balloons--;
          kid.isImune = true;
          kid.imuneTimer = 0;
          arduboy.audio.tone(300, 100);
          kid.speed.y = PLAYER_JUMP_VELOCITY;
          kid.speed.x = min((kid.pos.x - walkers[i].pos.x - 4), 3) << FIXED_POINT;
          //kid.speed.x = (kid.pos.x - spikes[i].pos.x - (spikes[i].pos.width >> 1) - 6);// << FIXED_POINT;
          //kid.speed.y = (kid.pos.y - spikes[i].pos.y - (spikes[i].pos.height >> 1) - 8);// << FIXED_POINT;
        }
      }
    }
    // Fans
    if (fans[i].active)
    {
      Rect fanrect = {.x = fans[i].pos.x, .y = fans[i].pos.y - (fans[i].height << 4),
      .width = 16, .height = fans[i].height << 4};
      if (kid.isBalloon && arduboy.collide(playerRect, fanrect))
      {
        kid.speed.y = min(kid.speed.y + FAN_POWER, MAX_YSPEED);
        if (arduboy.everyXFrames(3)) arduboy.audio.tone(330 + random(20), 30);
        //kid.actualpos.y -= FAN_POWER;
      }
    }

    // Spikes
    if (!kid.isImune && spikes[i].active && arduboy.collide(playerRect, spikes[i].pos))
    {
      if (kid.balloons == 1)
      {
        // dead
        gameState = STATE_GAME_OVER;
      }
      else
      {
        kid.balloons--;
        arduboy.audio.tone(300, 100);
        kid.isImune = true;
        kid.imuneTimer = 0;
        if (kid.pos.y < spikes[i].pos.y) kid.speed.y = PLAYER_JUMP_VELOCITY;
        //kid.speed.x = (kid.pos.x - spikes[i].pos.x - (spikes[i].pos.width >> 1) - 6);// << FIXED_POINT;
        //kid.speed.y = (kid.pos.y - spikes[i].pos.y - (spikes[i].pos.height >> 1) - 8);// << FIXED_POINT;
      }
    }
  }
}
#endif
