#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include "heavenlyBodies.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

#define TRANSLATE_X(x) (SCREEN_WIDTH/2 + x)
#define TRANSLATE_Y(y) (SCREEN_HEIGHT/2 - y)

// format: alpha/Blue/Green/Red
#define WHITE 0xFFFFFFFF
#define OFF_BLUE 0xFFE0DAAD
#define YELLOW 0xFF00FFFF
#define DARK_YELLOW 0xFF00CFCF
#define BLUE 0xFFFF0000
#define GREEN 0xFF00FF00
#define RED 0xFF0000FF

typedef struct {
    SDL_Rect rect;
    const char* text;
    SDL_Color color;
    SDL_bool isHovered;
} Button;

void renderGraphics(SDL_Renderer *renderer, TTF_Font* font, SolarSystem *solar_system);
void drawSun(SDL_Renderer *renderer, Star *sun);
void drawPlanets(SDL_Renderer *renderer, World **planets, int numPlanets);
void drawBackground(SDL_Renderer *renderer, TTF_Font* font);
void renderMenu(SDL_Renderer *renderer, TTF_Font *font, Button *play_button, Button *plus_planet_button, Button *minus_planet_button, int num_planets);
void renderButton(SDL_Renderer *renderer, TTF_Font *font, Button button);