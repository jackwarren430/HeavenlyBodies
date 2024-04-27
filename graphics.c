
#include "graphics.h"


void renderGraphics(SDL_Renderer *renderer, SolarSystem *solar_system){
    // draw the background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // draw the sun
    drawSun(renderer, solar_system->sun);

    // draw the planets
    drawPlanets(renderer, solar_system->planets, solar_system->num_planets);

    SDL_RenderPresent(renderer);
}

void drawSun(SDL_Renderer *renderer, Star *sun) {
    int x_coord = TRANSLATE_X(sun->coordinates[0]);
    int y_coord = TRANSLATE_Y(sun->coordinates[1]);
    float radius = sun->radius;
    filledCircleColor(renderer, x_coord, y_coord, radius, YELLOW);
}

void drawPlanets(SDL_Renderer *renderer, World **planets, int num_planets) {
    for (int i = 0; i < num_planets; i++) {
        World *cur_planet = planets[i];
        int x_coord = TRANSLATE_X(cur_planet->coordinates[0]);
        int y_coord = TRANSLATE_Y(cur_planet->coordinates[1]);
        float radius = cur_planet->radius;
        filledCircleColor(renderer, x_coord, y_coord, radius, BLUE);
    }
}