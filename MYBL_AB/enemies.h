#ifndef ENEMIES_H
#define ENEMIES_H

#include <Arduino.h>
#include "globals.h"
#include "GameObjects.hpp"
//#include "vec2.h"

#define MAX_FAN_PARTICLES 4
#define FAN_POWER           5
#define FAN_UP              0
#define FAN_RIGHT           1
#define FAN_LEFT            2

#define SPIKES_LEFT         0
#define SPIKES_DOWN         1
#define SPIKES_RIGHT        2
#define SPIKES_UP           3

#ifndef LSTART
#define LSTART  0
#define LFINISH 1 << 5
#define LWALKER 2 << 5
#define LFAN    3 << 5
#define LSPIKES 4 << 5
#define LCOIN   5 << 5
#define LKEY    6 << 5
#endif

extern byte fanFrame;
extern byte keysActive;
extern byte coinsActive;
extern bool haveKey;

class Coin : public GameObject
{
public:
  Coin(int x, int y) : GameObject(x, y) {
    x += 2;
    objType = LCOIN;
    ++coinsActive;
  }
  void update(HighRect &playerRect, HighRect &playerSuckRect) override {
    HighRect coinrect = {.x = x, .y = y, .width = 10, .height = 12};
    
    if (kid.isSucking && collide(playerSuckRect, coinrect))
    {
      // Suck coin closer
      if (kid.direction)
        ++x;
      else
        --x;
    }
    else if (collide(playerRect, coinrect))
    {
      // Collect coin
      --coinsActive;
      ++coinsCollected;
      ++totalCoins;
      sound.tone(400, 200);
      if (coinsActive == 0)
      {
        #ifndef HARD_MODE
        scorePlayer += 500;
        #else
        scorePlayer += 1000;
        #endif
      }
      else
      {
        #ifndef HARD_MODE
        scorePlayer += 200;
        #else
        scorePlayer += 400;
        #endif
      }
      GameObjects::removeForegroundObject(this);
    }
  }

  void draw() override {
    sprites.drawOverwrite(x - cam.pos.x, y - cam.pos.y, elements, coinFrame);
  }
};

struct Door : public GameObject {
  bool locked;

public:
  Door(int x, int y) : locked(true), GameObject(x, y) {
    objType = LFINISH;
  }

  void unlock() {
    locked = false;
  }

  void update(HighRect &playerRect, HighRect &playerSuckRect) override {
    HighRect exitRect = {.x = x + 4, .y = y, .width = 8, .height = 16};

    if (collide(exitRect, playerRect) && arduboy.justPressed(UP_BUTTON) && !locked)
    {
      balloonsLeft = kid.balloons;
      scoreIsVisible = true;
      canPressButton = false;
      level++;
      gameState = STATE_GAME_NEXT_LEVEL;
    }
  }

  void draw() override {
    sprites.drawOverwrite(x - cam.pos.x, x - cam.pos.y, door, !locked);
  }
};

struct Key : public GameObject
{
  Key(int x, int y) : GameObject(x, y) {
    objType = LKEY;
  }

  void draw() override {
    sprites.drawOverwrite(x - cam.pos.x, y - cam.pos.y, elements, 4);
  }

  void update(HighRect &playerRect, HighRect &playerSuckRect) override {
    HighRect keyRect = {.x = x, .y = y, .width = 8, .height = 16};
    
    if (kid.isSucking && collide(playerSuckRect, keyRect))
    {
      // Suck coin closer
      if (kid.direction)
        ++x;
      else
        --x;
    }
    else if (collide(playerRect, keyRect))
    {
      --keysActive;
      haveKey = true;
      sound.tone(420, 200);
      if (keysActive == 0) {
        unlockDoor();
      }
      GameObjects::removeForegroundObject(this);
    }
  }

  void unlockDoor() {
    for (auto current = GameObjects::getForegroundHead(); current != nullptr; current = current->next) {

      if (current->objType == LFINISH) {
        Door *door = current;
        door->unlock();
        break;
      }
    }
  }
};

struct Walker : public GameObject
{
  int8_t direction;
  int8_t hp;
  bool hurt;

  Walker(int x, int y) : hp(30), direction(1), hurt(false), GameObject(x, y) {
    y += 8;
    objType = LWALKER;
  }

  void update(HighRect &playerRect, HighRect &playerSuckRect) override {
    // Movement
    int yR = y >> 4;
    int xR = (x + 4 + (direction * 5)) >> 4;
    if (arduboy.everyXFrames(2) && hp > 0 && !hurt) {
      if (!gridGetSolid(xR, yR)
          && gridGetSolid(xR, yR + 1)) {
        x += direction;
      }
      else {
        direction = -direction;
      }
    }

    // Collisions
    HighRect walkerrect = {.x = x, .y = y, .width = 8, .height = 8};
    if (collide(playerSuckRect, walkerrect) && kid.isSucking) {

      --hp;
      hurt = true;

      if (hp <= 0) {
        if (kid.direction) {
          ++x;

          if (x > kid.pos.x - 8) {
            if (kid.balloons < 3) {
              ++kid.balloons;
            } else {
              scorePlayer += 100;
            }

            scorePlayer += 50;
            sound.tone(200, 100);
            GameObjects::removeForegroundObject(this);
          }
        }
        else {
          --x;

          if (x < kid.pos.x + 16) {
            if (kid.balloons < 3) {
              ++kid.balloons;
            } else {
              scorePlayer += 100;
            }
            
            scorePlayer += 50;
            sound.tone(200, 100);
            GameObjects::removeForegroundObject(this);
          }
        }
      }
    } else {
      hurt = false;
    }

    // Hurt player
    if (collide(playerRect, walkerrect) && hp > 0 && !kid.isImune) {
      kidHurt();
      kid.speed.y = PLAYER_JUMP_VELOCITY;
      kid.speed.x = max(min((kid.pos.x - x - 2), 3), -3) << FIXED_POINT;
    }
  }

  void draw() override {
    sprites.drawOverwrite(x - cam.pos.x, y - cam.pos.y, walkerSprite, walkerFrame + (hp <= 0) * 2);
  }
};

struct Spike : public GameObject
{
  int w;
  int h;
  byte image;

  Spike(int x, int y, int cellsToSpan) : GameObject(x, y) {
    objType = LSPIKES;
    int len = 16 * (cellsToSpan + 1);
    // Solid above
    if (gridGetSolid(x, y - 1))
    {
      image = 3;
      w = len;
      h = 8;
    }
    // Solid below
    else if (gridGetSolid(x, y + 1))
    {
      image = 1;
      w = len;
      h = 8;
      y += 8;
    }
    // Solid left
    else if (gridGetSolid(x - 1, y))
    {
      image = 0;
      w = 8;
      h = len;
    }
    // Solid right
    else if (gridGetSolid(x + 1, y))
    {
      image = 2;
      w = 8;
      h = len;
      x += 8;
    }
  }

  void draw() override {
    int commonX = x - cam.pos.x;
    int commonY = y - cam.pos.y;
    sprites.drawOverwrite(commonX, commonY, sprSpikes,  image);
    if (!bitRead(image, 0)) {
      for (int i = 8; i < h; i += 8)
        sprites.drawOverwrite(commonX, commonY + i, sprSpikes,  image);
    }
    else {
      for (int i = 8; i < w; i += 8)
        sprites.drawOverwrite(commonX + i, commonY, sprSpikes,  image);
    }
  }

  void update(HighRect &playerRect, HighRect &playerSuckRect) override {
    HighRect spikes = { .x = x, .y = y, .width = w, .height = h };
    if (!kid.isImune && collide(playerRect, spikes))
    {
      kidHurt();
      if (kid.pos.y < y) kid.speed.y = PLAYER_JUMP_VELOCITY;
    }
  }
};

struct Fan : public GameObject
{
  vec2 particles[5];
  int height;
  uint8_t dir;
  HighRect particleBox;

  Fan(int x, int y, int height, int direction) : height(height), dir(direction), GameObject(x, y) {
    objType = LFAN;
    for (byte i = 0; i < MAX_FAN_PARTICLES; ++i) {
      particles[i] = vec2(random(16), random(16));
    }

    switch (dir) {
      case FAN_UP:
      particleBox.x = x;
      particleBox.y = y - height;
      particleBox.width = 16;
      particleBox.height = height;
      break;
      case FAN_RIGHT:
      particleBox.x = x + 16;
      particleBox.y = y;
      particleBox.width = height;
      particleBox.height = 16;
      break;
      default:
      particleBox.x = x - height;
      particleBox.y = y;
      particleBox.width = height;
      particleBox.height = 16;
    }
  }

  void update(HighRect &playerRect, HighRect &playerSuckRect) override {
    if (collide(playerRect, particleBox) && kid.isBalloon) {
      switch (dir)
      {
        case FAN_UP:
        kid.speed.y = min(kid.speed.y + FAN_POWER, MAX_YSPEED);
        break;
        case FAN_RIGHT:
        kid.speed.x = min(kid.speed.x + FAN_POWER, MAX_XSPEED_FAN);
        break;
        default:
        kid.speed.x = max(kid.speed.x - FAN_POWER, -MAX_XSPEED_FAN);
      }

      windNoise();
    }
  }

  void draw() override {
    if (arduboy.everyXFrames(2)) {
      for (byte i = 0; i < MAX_FAN_PARTICLES; ++i)
      {
        // Update Particles
        particles[i].y = (particles[i].y < height) ? particles[i].y + 6 : random(height >> 2);

        // Draw particles
        switch (dir)
        {
          case FAN_UP:
          sprites.drawErase(x + particles[i].x - cam.pos.x, y - particles[i].y - cam.pos.y, particle , 0);
          break;
          case FAN_RIGHT:
          sprites.drawErase(x + 16 + particles[i].y - cam.pos.x, y + 16 - particles[i].x - cam.pos.y, particle , 0);
          break;
          default:
          sprites.drawErase(x - particles[i].y - cam.pos.x, y + 16 - particles[i].x - cam.pos.y, particle , 0);
        }
      }
    }

    // Draw fan
    int _x = x - cam.pos.x;
    int _y = y - cam.pos.y;
    uint8_t foff = 3 * dir;
    sprites.drawOverwrite(_x, _y, fan, fanFrame + foff);
  }
};


#endif
