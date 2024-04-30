#include "ui.h"


Uint32 lastTick;
float deltaTime;
float elapsedTicks;
extern State state;
SDL_bool done;
SDL_Renderer *renderer;
TTF_Font *font;

int main(void) {
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window *win = SDL_CreateWindow("HeavenlyBodies", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!win) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
    }

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 0;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    done = SDL_FALSE;
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 0;
    }
    state = MENU_INIT;
    font = TTF_OpenFont("./Basic-Regular.ttf", 24);
    lastTick = SDL_GetTicks();
    deltaTime = 0.0f;
    elapsedTicks = 0;
    SDL_Delay(16);
    while (!done) {
        elapsedTicks = SDL_GetTicks() - lastTick;
        deltaTime = elapsedTicks * 0.001f; 
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            } else {
                handleEvent(&event);
            }
        }
        if (deltaTime >= 1.0f / FRAMERATE) {
            mainLoop(deltaTime, renderer, font);
            lastTick = SDL_GetTicks();
            deltaTime = 0.0f;
            elapsedTicks = 0;
        }
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}











