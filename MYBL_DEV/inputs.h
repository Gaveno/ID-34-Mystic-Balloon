#ifndef INPUT_H
#define INPUT_H

extern Arduboy arduboy;
SimpleButtons buttons (arduboy);

void checkInputs()
{
  if (buttons.justPressed(RIGHT_BUTTON)){}
  if (buttons.justPressed(LEFT_BUTTON)){}
  if (buttons.justPressed(UP_BUTTON)){}
  if (buttons.justPressed(DOWN_BUTTON)){}
  if (buttons.justPressed(A_BUTTON)){}
  if (buttons.justPressed(B_BUTTON)){}
}

#endif
