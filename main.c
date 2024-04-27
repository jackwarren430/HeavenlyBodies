#include "physics.h"


SolarSystem *solar_system;
SDL_Renderer *renderer;

Uint32 lastTick;
float deltaTime;
float elapsedTicks;

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
            }
        }
        if (deltaTime >= 1.0f / FRAMERATE) {
            mainLoop();
            lastTick = SDL_GetTicks();
            deltaTime = 0.0f;
            elapsedTicks = 0;
        }

        
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

int initializeSystem() {
    solar_system = createHomogenousSolarSystem(10, PI / 3);
    //solar_system = createTwoBodySystem();
    //solar_system = createTwoPlanetSystem();
    if (!solar_system) {
        return FAILURE;
    }
    return SUCCESS;
}

void mainLoop() {
    updateSystemPhysics(solar_system, deltaTime);
    updateSystemLife(solar_system, deltaTime);
    renderGraphics(renderer, solar_system);

    /*float velocity_x = solar_system->planets[0]->velocity[0];
    float velocity_y = solar_system->planets[0]->velocity[1];
    float coord_x = solar_system->planets[0]->coordinates[0];
    float coord_y = solar_system->planets[0]->coordinates[1];

    printf("planet info:\nvelocity: %f, %f\nposition: %f, %f\n\n", velocity_x, velocity_y, coord_x, coord_y);
    */
}









