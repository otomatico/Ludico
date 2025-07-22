#ifndef _DISPLAY4B_H_
#define _DISPLAY4B_H_
#include "Canvas4bit.cpp"
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 40
Canvas4bits *DisplaySigleton =  new Canvas4bits(SCREEN_WIDTH,SCREEN_HEIGHT);
#endif