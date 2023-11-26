#ifndef _HIGH_RECT_H
#define _HIGH_RECT_H

#include "vec2.h"
// This is a replacement for struct Rect in the Arduboy2 library.
// It defines height as an int instead of a uint8_t to allow a higher rectangle.
struct HighRect
{
  public:
    int x;
    int y;
    uint16_t width;
    int height;

    HighRect() {
      x = 0;
      y = 0;
      width = 0;
      height = 0;
    }

    HighRect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

    HighRect(vec2 pos, vec2 dimensions) {
      HighRect(pos.x, pos.y, dimensions.x, dimensions.y);
    }
};
#endif