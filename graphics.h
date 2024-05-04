#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include "heavenlyBodies.h"

#define SCREEN_WIDTH (float)1000
#define SCREEN_HEIGHT (float)800
#define SCREEN_RATIO SCREEN_HEIGHT / SCREEN_WIDTH

// format: alpha/Blue/Green/Red
#define WHITE 0xFFFFFFFF
#define OFF_BLUE 0xFFE0DAAD
#define YELLOW 0xFF00FFFF
#define DARK_YELLOW 0xFF00CFCF
#define BLUE 0xFFFF0000
#define GREEN 0xFF00FF00
#define RED 0xFF0000FF

//  button positions
#define PLAY_BUTTON_X SCREEN_WIDTH / 2 - 40
#define PLAY_BUTTON_Y SCREEN_HEIGHT / 2 - 40
#define SOLAR_SYSTEM_Y SCREEN_HEIGHT / 2 + 150

#define PLUS_PLANET_X SCREEN_WIDTH / 5 - 50
#define MINUS_PLANET_X SCREEN_WIDTH / 5 + 30
#define PLUS_SUNMASS_X 2 * SCREEN_WIDTH / 5 - 50
#define MINUS_SUNMASS_X 2 * SCREEN_WIDTH / 5 + 60
#define PLUS_PLANETMASS_X 3 * SCREEN_WIDTH / 5 - 50
#define MINUS_PLANETMASS_X 3 * SCREEN_WIDTH / 5 + 30
#define PLUS_DISTANCE_X 4 * SCREEN_WIDTH / 5 - 50
#define MINUS_DISTANCE_X 4 * SCREEN_WIDTH / 5 + 30

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
void renderMenu(SDL_Renderer *renderer);
void renderButton(SDL_Renderer *renderer, TTF_Font *font, Button button);
void renderText(SDL_Renderer *renderer, TTF_Font *font, int x, int y, char *text);
void renderTimeFrame(SDL_Renderer *renderer, TTF_Font *font, float time);
void zoomOut();
void zoomIn();
float translateX(float x);
float translateY(float y);
float translateR(float r);
