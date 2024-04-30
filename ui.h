
#include "physics.h"

typedef enum {
    MENU_INIT,
    MENU,
    PLAY_INIT,
    PLAY,
    PAUSE_INIT,
    PAUSE,
} State;



SolarSystem *initializeSystem();
void mainLoop(float deltaTime, SDL_Renderer *renderer, TTF_Font *font);
SolarSystem *initializeSystem();
Button *createButton(int x, int y, int w, int h, const char *text, SDL_Color color);
void handleEvent(SDL_Event *event);
void handleMenuButtonEvent(SDL_Event *event);

