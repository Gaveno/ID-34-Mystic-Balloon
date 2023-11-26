#ifndef ENEMIES_H
#define ENEMIES_H

#include <Arduino.h>
#include "globals.h"
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

extern byte fanFrame;

class GameObjects {
  vec2 pos;
  GameObjects *mNext;
  static GameObjects *smObjectListFore = nullptr;
  static GameObjects *smObjectListBack = nullptr;

  static void addObj(GameObj *mObj, GameObjects **mList) {
    GameObjects **mCurrent = mList;
    while (*mCurrent != nullptr) {
      *mCurrent = (*mCurrent)->mNext;
    }
    *mCurrent = mObj;
  }

  static void clearList(GameObjects **mList) {
    while (*mList != nullptr) {
      GameObjects *mCurrent = &mList;
      *mList = mCurrent->mNext;
      delete mCurrent;
    }
  }

public:
  GameObjects() : pos(0, 0), mNext(nullptr) {}
  GameObjects(vec2 cellpos) : pos(cellpos << 4), mNext(nullptr) {}
  virtual void update();
  virtual void draw();

  static void removeForegroundObject(GameObjects *mObj) {
    GameObjects **mPrev = &smObjectListFore;
    while (*mPrev != nullptr && (*mPrev)->mNext != nullptr) {
      if (*mPrev == mObj) {
        *mPrev = mObj->mNext;
        delete mObj;
        break;
      }
    }
  }

  static void clearBackground() {
    clearList(smObjectListBack);
  }

  static void clearForeground() {
    clearList(smObjectListFore);
  }

  static void addBackgroundObject(GameObjects *mObj) {
    addObj(mObj, &smObjectListBack);
  }

  static void addForegroundObject(GameObjects *mObj) {
    addObj(mObj, &smObjectListFore);
  }

  static void updateObjects() {
    GameObjects **mCurrent = &smObjectListBack;
    while (*mCurrent != nullptr) {
      (*mCurrent)->update();
      *mCurrent = (*mCurrent)->mNext;
    }
    mCurrent = &smObjectListFore;
    while (*mCurrent != nullptr) {
      (*mCurrent)->update();
      *mCurrent = (*mCurrent)->mNext;
    }
  }

  static void drawObjects() {
    GameObjects **mCurrent = &smObjectListBack;
    while (*mCurrent != nullptr) {
      (*mCurrent)->update();
      *mCurrent = (*mCurrent)->mNext;
    }
    mCurrent = &smObjectListFore;
    while (*mCurrent != nullptr) {
      (*mCurrent)->update();
      *mCurrent = (*mCurrent)->mNext;
    }
  }
};



class Coin: public GameObjects
{
public:
  Coin(vec2 cellPos) : GameObjects(cellPos) {
    pos.x += 2;
  }
  void update() override {
    HighRect coinrect = {.x = pos.x, .y = pos.y, .width = 10, .height = 12};
    
    if (kid.isSucking && collide(playerSuckRect, coinrect))
    {
      // Suck coin closer
      if (kid.direction)
        ++pos.x;
      else
        --pos.x;
    }
    else if (collide(playerRect, coinrect))
    {
      // Collect coin
      coins[i].active = false; // TO-DO: Remove
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
        //sound.tone(400, 200);
      }
      else
      {
        #ifndef HARD_MODE
        scorePlayer += 200;
        #else
        scorePlayer += 400;
        #endif
        //sound.tone(370, 200);
      }
    }
  }

  void draw() override {
    sprites.drawOverwrite(pos.x - cam.pos.x, pos.y - cam.pos.y, elements, coinFrame);
  }
};

// Coin coins[MAX_PER_TYPE];

struct Door: public GameObjects {
  bool locked;

public:
  Door(vec2 cellPos) : locked(true), GameObjects(cellPos) {}

  void unlock() {
    locked = false;
  }

  void draw() override {
    sprites.drawOverwrite(pos.x - cam.pos.x, pos.x - cam.pos.y, door, !locked);
  }
};

struct Key: public GameObjects
{
  Door *pDoor

  Key(vec2 cellPos, bool haveKey) : haveKey(haveKey), GameObjects(cellPos) {}
};

Key key = {.pos = vec2(0, 0), .haveKey = false};

struct Walker
{
  vec2 pos;
  int8_t direction;
  int8_t HP;
  bool hurt;
  bool active;
};

Walker walkers[MAX_PER_TYPE];

struct Spike
{
  HighRect pos;
  byte characteristics;//B00000000;   //this byte holds all the characteristics for the spike
  //                      ||||||||
  //                      |||||||└->  0 \ these 2 bits are used to determine the spike type
  //                      ||||||└-->  1 /  
  //                      |||||└--->  2   the spike is active    (0 = false / 1 = true)
};

  Spike spikes[MAX_PER_TYPE];

struct Fan
{
  vec2 pos;
  vec2 particles[5];
  int height;
  bool active;
  uint8_t dir;
};

Fan fans[MAX_PER_TYPE];

void enemiesInit()
{
  coinsActive = 0;
  //for (byte i = 0; i < MAX_PER_TYPE; ++i)
  for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    // Fans
    fans[i].pos = vec2(0, 0);
    for (byte a = 0; a < MAX_FAN_PARTICLES; ++a)
      fans[i].particles[a] = vec2(random(16), random(16));
    fans[i].height = 0;
    fans[i].active = false;
    fans[i].dir = FAN_UP;

    // Spikes
    spikes[i].pos.x = 0;
    spikes[i].pos.y = 0;
    spikes[i].pos.width = 16;
    spikes[i].pos.height = 16;
    
    spikes[i].characteristics = 1;
    
    // Walkers
    walkers[i].pos.x = 0;
    walkers[i].pos.y = 0;
    walkers[i].active = false;
    walkers[i].HP = 30;
    walkers[i].direction = 1;
    walkers[i].hurt = false;

    // Coins
    coins[i].pos.x = 0;
    coins[i].pos.y = 0;
    coins[i].active = false;
  }
}

void keyCreate(vec2 pos)
{
  key.pos = pos << 4;
  key.active = true;
  key.haveKey = false;
}

void walkersCreate(vec2 pos)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  //for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (!walkers[i].active)
    {
      walkers[i].pos = pos << 4;
      walkers[i].pos.y += 8;
      walkers[i].active = true;
      return;
    }
  }
}

void spikesCreate(vec2 pos, byte cellsToSpan)
{
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    if (!bitRead(spikes[i].characteristics, 2))
    {
      int len = 16 * (cellsToSpan + 1);
      spikes[i].pos.x = pos.x << 4;
      spikes[i].pos.y = pos.y << 4;
      // Solid above
      if (gridGetSolid(pos.x, pos.y - 1))
      {
        spikes[i].characteristics = B00000111;
        spikes[i].pos.width = len;
        spikes[i].pos.height = 8;
      }
      // Solid below
      else if (gridGetSolid(pos.x, pos.y + 1))
      {
        spikes[i].characteristics = B00000101;
        spikes[i].pos.width = len;
        spikes[i].pos.height = 8;
        spikes[i].pos.y += 8;
      }
      // Solid left
      else if (gridGetSolid(pos.x - 1, pos.y))
      {
        spikes[i].characteristics = B00000100;
        spikes[i].pos.width = 8;
        spikes[i].pos.height = len;
      }
      // Solid right
      else if (gridGetSolid(pos.x + 1, pos.y))
      {
        spikes[i].characteristics = B00000110;
        spikes[i].pos.width = 8;
        spikes[i].pos.height = len;
        spikes[i].pos.x += 8;
      }
      return;
    }
  }
}

void fansCreate(vec2 pos, byte height, uint8_t dir = FAN_UP)
{
  //for (byte i = 0; i < MAX_PER_TYPE; ++i)
  for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (!fans[i].active)
    {
      fans[i].pos = pos << 4;
      fans[i].height = height << 4;
      fans[i].active = true;
      fans[i].dir = dir;
      return;
    }
  }
}

void enemiesUpdate()
{
  if (arduboy.everyXFrames(8))
  {
    walkerFrame = (++walkerFrame) % 2;
    coinFrame = (++coinFrame) % 4;
  }

  if (key.active)
  {
    int commonx = key.pos.x - cam.pos.x;
    int commony = key.pos.y - cam.pos.y;
    sprites.drawOverwrite(commonx, commony, elements, 4);
  }

  // Draw spikes first
  //for (byte i = 0; i < MAX_PER_TYPE; ++i)
  for (byte i = MAX_PER_TYPE-1; i < MAX_PER_TYPE; --i)
  {
    if (bitRead(spikes[i].characteristics, 2)) // spike active
    {
      int commonx = spikes[i].pos.x - cam.pos.x;
      int commony = spikes[i].pos.y - cam.pos.y;
      sprites.drawOverwrite(commonx, commony, sprSpikes,  spikes[i].characteristics & B00000011);
      if (!bitRead(spikes[i].characteristics, 0)) {
        for (int l = 8; l < spikes[i].pos.height; l += 8)
          sprites.drawOverwrite(commonx, commony + l, sprSpikes,  spikes[i].characteristics & B00000011);
      }
      else {
        for (int l = 8; l < spikes[i].pos.width; l += 8)
          sprites.drawOverwrite(commonx + l, commony, sprSpikes,  spikes[i].characteristics & B00000011);
      }
    }
  }

  if (arduboy.everyXFrames(4)) fanFrame = (++fanFrame) % 3;
  for (byte i = 0; i < MAX_PER_TYPE; ++i)
  {
    // Fans
    if (fans[i].active)
    {
      // Update
      if (arduboy.everyXFrames(2))
        for (byte a = 0; a < MAX_FAN_PARTICLES; ++a)
        {
          // Update Particles
          fans[i].particles[a].y =
            (fans[i].particles[a].y < (fans[i].height)) ?
            fans[i].particles[a].y + 6 : random((fans[i].height) >> 2);

          // Draw particles
          switch (fans[i].dir)
          {
            case FAN_UP:
            sprites.drawErase(fans[i].pos.x + fans[i].particles[a].x - cam.pos.x, fans[i].pos.y - fans[i].particles[a].y - cam.pos.y, particle , 0);
            break;
            case FAN_RIGHT:
            sprites.drawErase(fans[i].pos.x + 16 + fans[i].particles[a].y - cam.pos.x, fans[i].pos.y + 16 - fans[i].particles[a].x - cam.pos.y, particle , 0);
            break;
            default:
            sprites.drawErase(fans[i].pos.x - fans[i].particles[a].y - cam.pos.x, fans[i].pos.y + 16 - fans[i].particles[a].x - cam.pos.y, particle , 0);
          }
        }

      // Draw fan
      int _x = fans[i].pos.x - cam.pos.x;
      int _y = fans[i].pos.y - cam.pos.y;
      uint8_t foff = 3 * fans[i].dir;
      //if (fans[i].dir > FAN_UP) foff += 3;
      //else if (fans[i].dir > FAN_RIGHT) foff += 6;
      sprites.drawOverwrite(_x, _y, fan, fanFrame + foff);
    }

    // Walkers
    if (walkers[i].active)
    {
      if (arduboy.everyXFrames(2) && walkers[i].HP > 0 && !walkers[i].hurt)
      {
        if (!gridGetSolid((walkers[i].pos.x + 4 + (walkers[i].direction * 5)) >> 4, walkers[i].pos.y >> 4)
            && gridGetSolid((walkers[i].pos.x + 4 + (walkers[i].direction * 5)) >> 4, (walkers[i].pos.y >> 4) + 1))
        {
          walkers[i].pos.x += walkers[i].direction;
        }
        else
        {
          walkers[i].direction = -walkers[i].direction;
        }
      }

      sprites.drawOverwrite(walkers[i].pos.x - cam.pos.x, walkers[i].pos.y - cam.pos.y, walkerSprite, walkerFrame + (walkers[i].HP <= 0) * 2);
    }

    // Coins
    if (coins[i].active)
    {
      sprites.drawOverwrite(coins[i].pos.x - cam.pos.x, coins[i].pos.y - cam.pos.y, elements, coinFrame);
    }
  }
}


#endif
