#ifndef _VEC2.h
#define _VEC2.h

struct vec2
{
  int x;
  int y;

  vec2() : x(0), y(0) {}
  vec2(int vx, int vy) : x(vx), y(vy) {}

  vec2 &operator+=(const vec2 &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  vec2 &operator+(const vec2 &rhs) {
    *(this) += rhs;
    return *this;
  }
  vec2 &operator-=(const vec2 &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  vec2 &operator-(const vec2 &rhs) {
    *(this) -= rhs;
    return *this;
  }
  vec2 &operator>>(const int &rhs) {
    x >>= rhs;
    y >>= rhs;
    return *this;
  }
  vec2 &operator<<(const int &rhs) {
    x <<= rhs;
    y <<= rhs;
    return *this;
  }
};

#endif
