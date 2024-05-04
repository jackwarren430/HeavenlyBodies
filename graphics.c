
#include "graphics.h"

int view_width;
int y_offset;
int x_offset;
TTF_Font *numbers_font;
TTF_Font *font;
TTF_Font *font_big;

int num_planets;
int sun_mass;
int planet_mass_mult;
int distance;

// buttons
Button *play_button;
Button *plus_planet_button;
Button *minus_planet_button;
Button *plus_sun_mass_button;
Button *minus_sun_mass_button;
Button *plus_planet_mass_button;
Button *minus_planet_mass_button;
Button *plus_distance_button;
Button *minus_distance_button;


void renderGraphics(SDL_Renderer *renderer, TTF_Font* font, SolarSystem *solar_system){
    // draw the background
    drawBackground(renderer, font);
    

    // draw the sun
    if (solar_system->sun) {
        drawSun(renderer, solar_system->sun);
    }

    // draw the planets
    drawPlanets(renderer, solar_system->planets, solar_system->num_planets);

}

void drawBackground(SDL_Renderer *renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 172, 218, 224, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


    SDL_RenderDrawLine(renderer, translateX(0), 0, translateX(0), SCREEN_HEIGHT);
    SDL_RenderDrawLine(renderer, 0, translateY(0), SCREEN_WIDTH, translateY(0));


    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
    
    int num_lines = 20 * view_width / SCREEN_WIDTH;

    for (int i = 0; i < num_lines; i++) {
        float x = translateX(i * 50);
        float neg_x = translateX(-1 * i * 50);
        float y = translateY(i * 50);
        float neg_y = translateY(-1 * i * 50);
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
        SDL_RenderDrawLine(renderer, 0, neg_y, SCREEN_WIDTH, neg_y);
        SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer, neg_x, 0, neg_x, SCREEN_HEIGHT);
    }

    if (font == NULL) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return;
    }
    SDL_Color textColor = {0, 0, 0, 70};


    
    // numbers in positive x direction
    for (int i = 0; i < num_lines; i++) {
        char num_text[10];
        sprintf(num_text, "%d", i);
        SDL_Surface *textSurface = TTF_RenderText_Solid(numbers_font, num_text, textColor);
        if (textSurface == NULL) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture == NULL) {
            fprintf(stderr, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            SDL_FreeSurface(textSurface);
            return;
        }
        char num_text_neg[10];
        sprintf(num_text_neg, "-%d", i);
        SDL_Surface *textSurfaceNeg = TTF_RenderText_Solid(numbers_font, num_text_neg, textColor);
        if (textSurfaceNeg == NULL) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }
        SDL_Texture* textTextureNeg = SDL_CreateTextureFromSurface(renderer, textSurfaceNeg);
        if (textTextureNeg == NULL) {
            fprintf(stderr, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            SDL_FreeSurface(textSurfaceNeg);
            return;
        }

        int textWidthPos = textSurface->w;
        int textHeightPos = textSurface->h;
        int textWidthNeg = textSurfaceNeg->w;
        int textHeightNeg = textSurfaceNeg->h;

        float x = translateX(i * 50 + 3);
        float neg_x = translateX(-1 * i * 50);
        float y = translateY(i * 50 + 3);
        float neg_y = translateY(-1 * i * 50);

        SDL_Rect renderQuadPosX = {x, translateY(0), textWidthPos, textHeightPos};
        SDL_Rect renderQuadNegX = {neg_x, translateY(0), textWidthNeg, textHeightNeg};
        SDL_Rect renderQuadPosY = {translateX(3), y, textWidthPos, textHeightPos};
        SDL_Rect renderQuadNegY = {translateX(3), neg_y, textWidthNeg, textHeightNeg};

        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuadPosX);
        if (i != 0) {
            SDL_RenderCopy(renderer, textTextureNeg, NULL, &renderQuadNegX);
            SDL_RenderCopy(renderer, textTexture, NULL, &renderQuadPosY);
            SDL_RenderCopy(renderer, textTextureNeg, NULL, &renderQuadNegY);
        }
        

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTextureNeg);
        SDL_FreeSurface(textSurfaceNeg);

    }
    
}

void drawSun(SDL_Renderer *renderer, Star *sun) {
    float x_coord = translateX(sun->coordinates[0]);
    float y_coord = translateY(sun->coordinates[1]);
    float radius = translateR(sun->radius);
    filledCircleColor(renderer, x_coord, y_coord, radius, YELLOW);
}

void drawPlanets(SDL_Renderer *renderer, World **planets, int num_planets) {
    for (int i = 0; i < num_planets; i++) {
        World *cur_planet = planets[i];
        float x_coord = translateX(cur_planet->coordinates[0]);
        float y_coord = translateY(cur_planet->coordinates[1]);
        float radius = translateR(cur_planet->radius);
        
        //printf("ty: %f\ntest: %f\ntest2: %f\ntest3: %f\ntest4: %f\n\n", y_coord, test, test2, test3, test4);
        filledCircleColor(renderer, x_coord, y_coord, radius, BLUE);
        
    }
}

void renderMenu(SDL_Renderer *renderer) {
    if (!renderer || !font || !play_button || !plus_planet_button || !minus_planet_button) {
        printf("Error: passed in a null pointer to renderMenu\n");
        return;
    }

    SDL_SetRenderDrawColor(renderer, 172, 218, 224, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    renderButton(renderer, font, *play_button);
    renderButton(renderer, font, *plus_planet_button);
    renderButton(renderer, font, *minus_planet_button);
    renderButton(renderer, font, *plus_sun_mass_button);
    renderButton(renderer, font, *minus_sun_mass_button);
    renderButton(renderer, font, *plus_planet_mass_button);
    renderButton(renderer, font, *minus_planet_mass_button);
    renderButton(renderer, font, *plus_distance_button);
    renderButton(renderer, font, *minus_distance_button);

    // number of planets
    char num_text[5];
    sprintf(num_text, "%d", num_planets);
    int x_pos = PLUS_PLANET_X + 40;
    int y_pos = SOLAR_SYSTEM_Y;
    renderText(renderer, font, x_pos, y_pos, num_text);
    char *d_num_text = "# planets:";
    x_pos -= 50;
    y_pos -= 40;
    renderText(renderer, font, x_pos, y_pos, d_num_text);

    //  title text
    char *menu_text = "HeavenlyBodies Simulator";
    x_pos = 160;
    y_pos = 120;
    renderText(renderer, font_big, x_pos, y_pos, menu_text);

    // sun mass text
    char sun_mass_text[10];
    sprintf(sun_mass_text, "%d", (int)sun_mass);
    x_pos = PLUS_SUNMASS_X + 40;
    y_pos = SOLAR_SYSTEM_Y;
    renderText(renderer, font, x_pos, y_pos, sun_mass_text);
    char *d_sun_mass_text = "Sun mass:";
    x_pos -= 50;
    y_pos -= 40;
    renderText(renderer, font, x_pos, y_pos, d_sun_mass_text);

    // planet mass text
    char planet_mass_text[10];
    sprintf(planet_mass_text, "%d", (int)planet_mass_mult);
    x_pos = PLUS_PLANETMASS_X + 40;
    y_pos = SOLAR_SYSTEM_Y;
    renderText(renderer, font, x_pos, y_pos, planet_mass_text);
    char *d_planet_mass_text = "planet mass mult:";
    x_pos -= 50;
    y_pos -= 40;
    renderText(renderer, font, x_pos, y_pos, d_planet_mass_text);

    // distance text
    char distance_text[10];
    x_pos = PLUS_DISTANCE_X + 40;
    y_pos = SOLAR_SYSTEM_Y;
    sprintf(distance_text, "%d", (int)distance);
    renderText(renderer, font, x_pos, y_pos, distance_text);
    char *d_distance_text = "Distance interval:";
    x_pos -= 50;
    y_pos -= 40;
    renderText(renderer, font, x_pos, y_pos, d_distance_text);
}


void renderButton(SDL_Renderer *renderer, TTF_Font *font, Button button) {
    SDL_Color color = button.isHovered ? (SDL_Color){200, 200, 200, 255} : button.color;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &button.rect);
    SDL_Surface* surface = TTF_RenderText_Solid(font, button.text, (SDL_Color){0, 0, 0, 255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {
        button.rect.x + (button.rect.w - surface->w) / 2,
        button.rect.y + (button.rect.h - surface->h) / 2,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

}

void renderText(SDL_Renderer *renderer, TTF_Font *font, int x, int y, char *text) {
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        fprintf(stderr, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect renderQuad = {x, y, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void renderTimeFrame(SDL_Renderer *renderer, TTF_Font *font, float time) {
    SDL_Color textColor = {0, 0, 0, 255};
    char num_text[30];
    sprintf(num_text, "Time Frame: %.2f", time);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, num_text, textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        fprintf(stderr, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    int x_pos = 200;
    int y_pos = 60;
    SDL_Rect renderQuad = {x_pos, y_pos, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void zoomOut() {
    view_width *= 1.2;
    numbers_font = TTF_OpenFont("./Basic-Regular.ttf", 24 * SCREEN_WIDTH / view_width);
}

void zoomIn() {
    view_width *= 0.8;
    numbers_font = TTF_OpenFont("./Basic-Regular.ttf", 24 * SCREEN_WIDTH / view_width);
}

float translateX(float x) {
    float test2 = (SCREEN_WIDTH/2);
    float test3 = (view_width/2);
    float test4 = test2 / test3;
    float ret = SCREEN_WIDTH/2 + (x * test4);
    return ret + x_offset;
}

float translateY(float y) {
    float test2 = (SCREEN_HEIGHT/2);
    float test3 = ((view_width * SCREEN_RATIO)/2);
    float test4 = test2 / test3;
    float ret = SCREEN_HEIGHT/2 - (y * test4);
    return ret + y_offset;
}

float translateR(float r) {
    float t1 = (SCREEN_WIDTH / view_width);
    float ret = r * t1;
    return ret;
}



