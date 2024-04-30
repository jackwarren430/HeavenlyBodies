
#include "graphics.h"


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
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);
    SDL_RenderDrawLine(renderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
    for (int i = 0; i < 16; i++) {
        SDL_RenderDrawLine(renderer, 0, i * (SCREEN_HEIGHT / 16), SCREEN_WIDTH, i * (SCREEN_HEIGHT / 16));
    }
    for (int i = 0; i < 20; i++) {
        SDL_RenderDrawLine(renderer, i * (SCREEN_WIDTH / 20), 0, i * (SCREEN_WIDTH / 20), SCREEN_HEIGHT);
    }

    if (font == NULL) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return;
    }
    SDL_Color textColor = {0, 0, 0, 70};


    
    // numbers in positive x direction
    for (int i = 0; i < 10; i++) {
        char num_text[3];
        sprintf(num_text, "%d", i);
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
        int x_pos = (SCREEN_WIDTH / 2) + (i * (SCREEN_WIDTH / 20)) + 3;
        int y_pos = (SCREEN_HEIGHT / 2) + 2;
        SDL_Rect renderQuad = {x_pos, y_pos, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }
    

    // numbers in negative x direction
    for (int i = 1; i < 10; i++) {
        char num_text[3];
        sprintf(num_text, "-%d", i);
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
        int x_pos = (SCREEN_WIDTH / 2) - (i * (SCREEN_WIDTH / 20)) + 3;
        int y_pos = (SCREEN_HEIGHT / 2) + 2;
        SDL_Rect renderQuad = {x_pos, y_pos, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }

    // numbers in positive y direction
    for (int i = 1; i < 8; i++) {
        char num_text[3];
        sprintf(num_text, "%d", i);
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
        int x_pos = (SCREEN_WIDTH / 2) + 4;
        int y_pos = (SCREEN_HEIGHT / 2) - (i * (SCREEN_HEIGHT / 16)) + 2;
        SDL_Rect renderQuad = {x_pos, y_pos, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }

    for (int i = 1; i < 8; i++) {
        char num_text[3];
        sprintf(num_text, "-%d", i);
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
        int x_pos = (SCREEN_WIDTH / 2) + 4;
        int y_pos = (SCREEN_HEIGHT / 2) + (i * (SCREEN_HEIGHT / 16)) + 2;
        SDL_Rect renderQuad = {x_pos, y_pos, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }
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
        char *target = "target";
        if (strcmp(cur_planet->name, target) == 0) {
            filledCircleColor(renderer, x_coord, y_coord, radius, RED);
        } else {
            filledCircleColor(renderer, x_coord, y_coord, radius, BLUE);
        }
        
    }
}

void renderMenu(SDL_Renderer *renderer, TTF_Font *font, Button *play_button, Button *plus_planet_button, Button *minus_planet_button, int num_planets) {
    SDL_SetRenderDrawColor(renderer, 172, 218, 224, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    renderButton(renderer, font, *play_button);
    renderButton(renderer, font, *plus_planet_button);
    renderButton(renderer, font, *minus_planet_button);

    SDL_Color textColor = {0, 0, 0, 255};
    char num_text[3];
    sprintf(num_text, "%d", num_planets);
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
    int x_pos = (SCREEN_WIDTH / 2) - 5;
    int y_pos = (SCREEN_HEIGHT / 2) + 30;
    SDL_Rect renderQuad = {x_pos, y_pos, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
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

