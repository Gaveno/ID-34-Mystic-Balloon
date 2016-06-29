#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <Arduino.h>
#include "globals.h"

#define FONT_TINY                 0
#define FONT_SMALL                1
#define FONT_BIG                  2

#define DATA_TIMER                0
#define DATA_SCORE                1
#define DATA_LEVEL                2

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

void drawNumbers(byte numbersX, byte numbersY, byte fontType, byte data)
{
  char buf[10];
  char charLen;
  char pad;
  switch (data)
  {
    case DATA_SCORE:
      ltoa(scorePlayer, buf, 10);
      charLen = strlen(buf);
      pad = 6 - charLen;
      sprites.drawSelfMasked(numbersX - 2, numbersY - 2, numbersBigMask00, 0);
      for (byte i = 0; i < 6; i++)sprites.drawSelfMasked(numbersX + (7 * i), numbersY - 2, numbersBigMask01, 0);
      sprites.drawSelfMasked(numbersX + 41, numbersY - 2, numbersBigMask02, 0);
      break;
    case DATA_LEVEL:
      itoa(level + 1, buf, 10);
      charLen = strlen(buf);
      pad = 2 - charLen;
      sprites.drawSelfMasked(numbersX-2, numbersY - 2, badgeLevel, 0);
      
      break;
  }

  //draw 0 padding
  for (byte i = 0; i < pad; i++)
  {
    switch (fontType)
    {
      case FONT_SMALL:
        sprites.drawErase(numbersX + (6 * i), numbersY, elementsHUD, 0);
        break;
      case FONT_BIG:
        sprites.drawSelfMasked(numbersX + (7 * i), numbersY, numbersBig, 0);
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
      case FONT_SMALL:
        sprites.drawErase(numbersX + (pad * 6) + (6 * i), numbersY, elementsHUD, digit);
        break;
      case FONT_BIG:
        sprites.drawSelfMasked(numbersX + (pad * 7) + (7 * i), numbersY, numbersBig, digit);
        break;
    }
  }
}

#endif
