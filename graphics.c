
#include "graphics.h"


void renderGraphics(SDL_Renderer *renderer, SolarSystem *solar_system){
    // draw the background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // draw the sun
    if (solar_system->sun) {
        drawSun(renderer, solar_system->sun);
    }

    // draw the planets
    drawPlanets(renderer, solar_system->planets, solar_system->num_planets);

    SDL_RenderPresent(renderer);
}

void drawSun(SDL_Renderer *renderer, Star *sun) {
    float x_coord = TRANSLATE_X(sun->coordinates[0]);
    float y_coord = TRANSLATE_Y(sun->coordinates[1]);
    float radius = sun->radius;
    filledCircleColor(renderer, x_coord, y_coord, radius, YELLOW);
}

void drawPlanets(SDL_Renderer *renderer, World **planets, int num_planets) {
    for (int i = 0; i < num_planets; i++) {
        World *cur_planet = planets[i];
        float x_coord = TRANSLATE_X(cur_planet->coordinates[0]);
        float y_coord = TRANSLATE_Y(cur_planet->coordinates[1]);
        float radius = cur_planet->radius;
        char *target_name = "planet_3";
        if (strcmp(cur_planet->name, target_name) == 0) {
            filledCircleColor(renderer, x_coord, y_coord, radius, GREEN);
        } else {
            filledCircleColor(renderer, x_coord, y_coord, radius, BLUE);
        }
        
    }
}