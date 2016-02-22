#ifndef STAGE_H
#define STAGE_H

#include "inputs.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO         0
#define STATE_MENU_MAIN          1
#define STATE_MENU_HELP          2
#define STATE_MENU_PLAY          3
#define STATE_MENU_INFO          4
#define STATE_MENU_SOUNDFX       5

//define game states (on main menu)
#define STATE_GAME_PLAYING       6
#define STATE_GAME_PAUSE         7
#define STATE_GAME_OVER          8

Sprites sprites(arduboy);

extern Arduboy arduboy;
extern byte gameState;

void stateGamePlaying()
{
  checkInputs();
}
void stateGamePause()
{
  if (buttons.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
  }
}
void stateGameOver()
{
  if (buttons.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
  }
}

#endif
