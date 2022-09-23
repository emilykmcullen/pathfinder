#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const unsigned int FPS = 60;
const unsigned int FRAME_TARGET_TIME = 1000/FPS;

//BOXES

// MAKE SURE THAT WINDOW_WIDTH AND WINDOW_HEIGHT CAN BE DIVIDED BY THIS INTO WHOLE NUMBERS OR .5 OF A NUMBER
const unsigned int BOXES_PER_ROW_AND_COLUMN = 10; 
const unsigned int BOX_PADDING = 1;
const float BOX_WIDTH = WINDOW_WIDTH / BOXES_PER_ROW_AND_COLUMN;
const float BOX_HEIGHT = WINDOW_HEIGHT / BOXES_PER_ROW_AND_COLUMN;
// value used to create a 'midpoint key', it's based on the window_height, so if window_height is a 3 digit number, then should be 1000
// if window_height is a 4 digit number then this should be 10000, and so on
const unsigned int BOX_MIDPOINT_MULTIPLIER = 1000; 


#endif