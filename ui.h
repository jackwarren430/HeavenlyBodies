
#include "physics.h"

typedef enum {
    MENU_INIT,
    MENU,
    PLAY_INIT,
    PLAY,
    PLAYBACK_INIT,
    PLAYBACK,
    PLAYBACK_PAUSE_INIT,
    PLAYBACK_PAUSE,
} State;




SolarSystem *initializeSystem();
void mainLoop(float deltaTime, SDL_Renderer *renderer, TTF_Font *font, TTF_Font *font_big);
SolarSystem *initializeSystem();
Button *createButton(int x, int y, int w, int h, const char *text, SDL_Color color);
void handleEvent(SDL_Event *event);
void handleKeyboardEvent(SDL_Event *event);
void handleMenuButtonEvent(SDL_Event *event);
void handlePlayButtonEvent(SDL_Event *event);
void handlePlaybackButtonEvent(SDL_Event *event);
void handlePauseButtonEvent(SDL_Event *event);
void buttonIsHovered(Button *button, SDL_Event *event);

