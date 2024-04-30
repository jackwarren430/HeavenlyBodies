#include "ui.h"


SolarSystem *solar_system;
SDL_Renderer *renderer;
TTF_Font* font;

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
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_bool done = SDL_FALSE;
    if (initializeSystem() == FAILURE) {
        done = SDL_TRUE;
    }
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 0;
    }
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
            }
        }
        if (deltaTime >= 1.0f / FRAMERATE) {
            mainLoop();
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

int initializeSystem() {
    solar_system = createHomogenousSolarSystem(10, PI / 3, 30);
    //solar_system = createTwoBodySystem();
    //solar_system = createTwoPlanetSystem();
    //solar_system = createThreeBodyProblem();
    if (!solar_system) {
        return FAILURE;
    }
    return SUCCESS;
}

void mainLoop() {
    updateSystemPhysics(solar_system, deltaTime);
    updateSystemLife(solar_system, deltaTime);
    renderGraphics(renderer, font, solar_system);

    /*float velocity_x = solar_system->planets[0]->velocity[0];
    float velocity_y = solar_system->planets[0]->velocity[1];
    float coord_x = solar_system->planets[0]->coordinates[0];
    float coord_y = solar_system->planets[0]->coordinates[1];

    printf("planet info:\nvelocity: %f, %f\nposition: %f, %f\n\n", velocity_x, velocity_y, coord_x, coord_y);
    */
}









