#include "physics.h"


SolarSystem *solar_system;
SDL_Renderer *renderer;

int initializeSystem();
void mainLoop();

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

    SDL_bool done = SDL_FALSE;
    if (initializeSystem() == FAILURE) {
        done = SDL_TRUE;
    }
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = SDL_TRUE;
            }
        }
        mainLoop();
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

int initializeSystem() {
    solar_system = createHomogenousSolarSystem(10);
    if (!solar_system) {
        return FAILURE;
    }
    return SUCCESS;
}

void mainLoop() {
    updateSystemPhysics(solar_system);
    updateSystemLife(solar_system);
    renderGraphics(renderer, solar_system);
}









