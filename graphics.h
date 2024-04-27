#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include "heavenlyBodies.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700

#define TRANSLATE_X(x) (SCREEN_WIDTH/2 + x)
#define TRANSLATE_Y(y) (SCREEN_HEIGHT/2 - y)

// format: alpha/Blue/Green/Red
#define WHITE 0xFFFFFFFF
#define YELLOW 0xFF00FFFF
#define DARK_YELLOW 0xFF00CFCF
#define BLUE 0xFFFF0000

//#define TRANSLATE_X(int x)

void renderGraphics(SDL_Renderer *renderer, SolarSystem *solar_system);
void drawSun(SDL_Renderer *renderer, Star *sun);
void drawPlanets(SDL_Renderer *renderer, World **planets, int numPlanets);