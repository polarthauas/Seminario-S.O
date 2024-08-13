#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

#define DESIGN_WIDTH 1360
#define DESIGN_HEIGHT 768

extern int windowWidth, windowHeight;

//Padrão 800x600

#define BLOCK_WIDTH 50
#define BLOCK_HEIGHT 50

#define DOUGLAS_WIDTH 32
#define DOUGLAS_HEIGHT 32

int calcAlterWindowSize(int a, char mode);

bool clickedRect(int x, int y, SDL_Rect b);
