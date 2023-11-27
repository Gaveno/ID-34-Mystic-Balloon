#ifndef LEVELS_H
#define LEVELS_H

#include <Arduino.h>
#include "globals.h"
#include "enemies.h"
//#include "Point.h"
#include "player.h"

// Type IDs of game objects in level data
#ifndef LSTART
#define LSTART  0
#define LFINISH 1 << 5
#define LWALKER 2 << 5
#define LFAN    3 << 5
#define LSPIKES 4 << 5
#define LCOIN   5 << 5
#define LKEY    6 << 5
#endif


//char gameGrid[LEVEL_ARRAY_SIZE]; // grid with cell information
// upper byte tile xxxx ____
// LSB solid ____ ___x


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
  if (!gridGetSolid(x, y)) {
    return 16;
  }

  byte l, r, t, b, f;
  l = gridGetSolid(x - 1, y);
  t = gridGetSolid(x, y - 1);
  r = gridGetSolid(x + 1, y);
  b = gridGetSolid(x, y + 1);

  f = 0;
  f = r | (t << 1) | (l << 2) | (b << 3);

  return f;
}


void levelLoad(const uint8_t *lvl) {
  // Wipe old objects
  GameObjects::clearForeground();
  GameObjects::clearBackground();
  haveKey = false;
  keysActive = 0;
  coinsActive = 0;

  // Load level data
  byte i = 0;
  lvl += LEVEL_ARRAY_SIZE >> 3;

  byte b = pgm_read_byte(lvl);
  while (b != 0xFF)
  {
    byte id;
    int x, y;
    id = pgm_read_byte(lvl + i) & 0xE0;
    y = (int)(pgm_read_byte(lvl + i++) & 0x1F) << 4;
    x = (int)(pgm_read_byte(lvl + i++) & 0x1F) << 4;

    // #ifdef DEBUG_MESSAGES
    // Serial.println("x, y");
    // Serial.println(x);
    // Serial.println(y);
    // #endif

    switch ( id )
    {
      case LSTART:
        {
          // Start
          startPos.x = x << FIXED_POINT;
          startPos.y = y << FIXED_POINT;
          kid.actualpos = startPos;
          
          // #ifdef DEBUG_MESSAGES
          // Serial.println("Start");
          // #endif
        }
        break;
      case LFINISH:
        {
          // Finish/Door
          GameObjects::addBackgroundObject(new Door(x, y));

          // #ifdef DEBUG_MESSAGES
          // Serial.println("Exit Door");
          // #endif
        }
        break;
      case LWALKER:
        {
          // Walker
          GameObjects::addForegroundObject(new Walker(x, y));

          // #ifdef DEBUG_MESSAGES
          // Serial.println("Walker");
          // #endif
        }
        break;
      case LFAN:
        {
          // Fan
          byte t = pgm_read_byte(lvl + i++);
          byte direction = FAN_UP;
          if (t >= 64 && t < 192)
            direction = FAN_RIGHT;
          else if (t >= 192)
            direction = FAN_LEFT;
          GameObjects::addForegroundObject(new Fan(x, y, (t & 0x3F) << 4, direction));

          // #ifdef DEBUG_MESSAGES
          // Serial.println("Fan");
          // #endif
        }
        break;
      case LSPIKES:
        {
          // Spikes
          GameObjects::addBackgroundObject(new Spike(x, y, pgm_read_byte(lvl + (i - 1)) >> 5));

          // #ifdef DEBUG_MESSAGES
          // Serial.println("Spikes");
          // #endif
        }
        break;
      case LCOIN:
        {
          // Coins
          GameObjects::addForegroundObject(new Coin(x, y));

          // #ifdef DEBUG_MESSAGES
          // Serial.println("Coin");
          // #endif
        }
        break;
      default: //case LKEY:
        {
          // Key
          GameObjects::addForegroundObject(new Key(x, y));

          // #ifdef DEBUG_MESSAGES
          // Serial.println("Key");
          // #endif
        }
        break;
    }

    b = pgm_read_byte(lvl + i);
  }
}

void drawGrid() {
  int spacing = 16;

  for (byte x = 8; x < 9; --x) {
      for (byte y = 5; y < 6; --y) {
            sprites.drawSelfMasked(x * spacing - (cam.pos.x >> 2) % spacing, (int)y * spacing - ((cam.pos.y + 64) >> 2) % spacing, tileSetTwo, 16);
      }
  }

  int camXStart = cam.pos.x >> 4;
  int camYStart = cam.pos.y >> 4;
  int camXEnd = camXStart + 8;
  int camYEnd = camYStart + 4;
  for (int x = camXStart; x <= camXEnd; ++x) {
    for ( int y = camYStart; y <= camYEnd; ++y) {
      byte tile = gridGetTile(x, y);
      if (tile != 16) {
        sprites.drawOverwrite((x << 4) - cam.pos.x, (y << 4) - cam.pos.y, tileSetTwo, tile);
      }
    }
  }
}

void windNoise() {
  if (arduboy.everyXFrames(2)) sound.tone(320 + random(20), 30);
}

void kidHurt()
{
  if (kid.balloons == 0)
    return;
  
  kid.isBalloon = false;
  kid.balloons--;
  sound.tone(420, 100);
  kid.isImune = true;
  kid.imuneTimer = 0;
}

void drawHUD()
{
  for (byte i = 15; i < 16; --i)
  {
    sprites.drawSelfMasked(i * 8, 0, smallMask, 0);
  }
  drawBalloonLives();
  drawNumbers(91, 0, FONT_SMALL, DATA_SCORE);
  drawCoinHUD();
  if (haveKey) sprites.drawOverwrite(28, 0, elementsHUD, 13);
}
#endif
