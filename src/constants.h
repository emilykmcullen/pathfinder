#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const unsigned int FPS = 60;
const unsigned int FRAME_TARGET_TIME = 1000/FPS;

//BOXES
const unsigned int BOXES_PER_ROW_AND_COLUMN = 10;
const unsigned int BOX_PADDING = 1;
const float BOX_WIDTH = WINDOW_WIDTH / BOXES_PER_ROW_AND_COLUMN;
const float BOX_HEIGHT = WINDOW_HEIGHT / BOXES_PER_ROW_AND_COLUMN;


#endif