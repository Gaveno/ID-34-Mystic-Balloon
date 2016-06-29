#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <Arduino.h>
#include "globals.h"

void drawBalloonLives()
{
  for (byte i = 0; i < kid.balloons; ++i)
  {
    sprites.drawErase((i * 7) + 2, 0, elementsHUD, 10);
  }
}

void drawMiniMap(byte offset)
{
  byte offx, offy;
  offx = 0;
  offy = 40 + offset;
  for (byte x = 0; x < LEVEL_WIDTH_CELLS; ++x)
  {
    for (byte y = 0; y < LEVEL_HEIGHT_CELLS; ++y)
    {
      arduboy.drawPixel(offx + x, offy + y, !gridGetSolid(x, y));
    }
  }
  if (!walkerFrame)
  {
    //arduboy.drawPixel(offx + (startPos.x >> 4), offy + (startPos.y >> 4), 0);
    arduboy.drawPixel(offx + (levelExit.x >> 4), offy + (levelExit.y >> 4), 0);
  }
  if (fanFrame)
    arduboy.drawPixel(offx + ((kid.pos.x + 6) >> 4), offy + ((kid.pos.y + 8) >> 4), 0);
}

void drawCoinHUD()
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (i >= MAX_PER_TYPE - coinsActive)
      sprites.drawErase(40 + (i * 6), 0, elementsHUD, 11);
    else
      sprites.drawErase(40 + (i * 6), 0, elementsHUD, 12);
  }
}

void drawScore(byte scoreX, byte scoreY, byte fontType)
{
  if (fontType == SCORE_BIG_FONT)
  {
    sprites.drawSelfMasked(scoreX - 2, scoreY - 2, numbersBigMask00, 0);
    for (byte i = 0; i < 6; i++)sprites.drawSelfMasked(scoreX + (7 * i), scoreY - 2, numbersBigMask01, 0);
    sprites.drawSelfMasked(scoreX + 41, scoreY - 2, numbersBigMask02, 0);
  }
  char buf[10];
  //scorePlayer = arduboy.cpuLoad();
  ltoa(scorePlayer, buf, 10);
  char charLen = strlen(buf);
  char pad = 6 - charLen;

  //draw 0 padding
  for (byte i = 0; i < pad; i++)
  {
    switch (fontType)
    {
      case SCORE_SMALL_FONT:
        sprites.drawErase(scoreX + (6 * i), scoreY, elementsHUD, 0);
        break;
      case SCORE_BIG_FONT:
        sprites.drawSelfMasked(scoreX + (7 * i), scoreY, numbersBig, 0);
        break;
    }
  }

  for (byte i = 0; i < charLen; i++)
  {
    char digit = buf[i];
    byte j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (byte z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    switch (fontType)
    {
      case SCORE_SMALL_FONT:
        sprites.drawErase(scoreX + (pad * 6) + (6 * i), scoreY, elementsHUD, digit);
        break;
      case SCORE_BIG_FONT:
        sprites.drawSelfMasked(scoreX + (pad * 7) + (7 * i), scoreY, numbersBig, digit);
        break;
    }
  }
}

#endif
