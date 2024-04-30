#include "ui.h"

State state;
SolarSystem *solar_system;
extern SDL_bool done;
Button *play_button;
Button *plus_planet_button;
Button *minus_planet_button;
int num_planets;

void mainLoop(float deltaTime, SDL_Renderer *renderer, TTF_Font *font) {

    switch (state) {
    case MENU_INIT: {
        num_planets = 0;
        char *text = "Play";
        SDL_Color color = (SDL_Color){255, 255, 255, 255};
        play_button = createButton(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 40, 80, 40, text, color);
        plus_planet_button = createButton(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2 + 30, 30, 30, "+", color);
        minus_planet_button = createButton(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 30, 30, 30, "-", color);
        state = MENU;
        break;}
    case MENU: {
        renderMenu(renderer, font, play_button, plus_planet_button, minus_planet_button, num_planets);
        break;}
    case PLAY_INIT: {
        solar_system = initializeSystem();
        if (solar_system == NULL) {
            done = SDL_TRUE;
        }
        state = PLAY;
        break;}
    case PLAY: {
        updateSystemPhysics(solar_system, deltaTime);
        updateSystemLife(solar_system, deltaTime);
        renderGraphics(renderer, font, solar_system);
        break;}
    case PAUSE_INIT: {

        state = PAUSE;
        break; }
    case PAUSE: {
        renderGraphics(renderer, font, solar_system);
        break;}
    }
    SDL_RenderPresent(renderer);
}

SolarSystem *initializeSystem() {
    SolarSystem *solar_system = createHomogenousSolarSystem(num_planets, PI / 3, 20);
    //solar_system = createChaoticSolarSystem(100, PI / 10, 10);
    //solar_system = createTwoBodySystem();
    //solar_system = createTwoPlanetSystem();
    //solar_system = createThreeBodyProblem();
    if (!solar_system) {
        return NULL;
    }
    return solar_system;

}

void handleEvent(SDL_Event *event) {
    switch (state) {
    case MENU_INIT: break;
    case MENU: {
        handleMenuButtonEvent(event);
        break;}
    case PLAY_INIT: break;
    case PLAY: {

        break;}
    case PAUSE_INIT: break;
    case PAUSE: {

        break;}
    }
}

void handleMenuButtonEvent(SDL_Event *event) {
    if (event->type == SDL_MOUSEMOTION) {
        int x = event->motion.x;
        int y = event->motion.y;
        // play button
        play_button->isHovered = (x >= play_button->rect.x && x <= play_button->rect.x + play_button->rect.w &&
                             y >= play_button->rect.y && y <= play_button->rect.y + play_button->rect.h);
        // plus planet button
        plus_planet_button->isHovered = (x >= plus_planet_button->rect.x && x <= plus_planet_button->rect.x + plus_planet_button->rect.w &&
                             y >= plus_planet_button->rect.y && y <= plus_planet_button->rect.y + plus_planet_button->rect.h);
        // minus planet button
        minus_planet_button->isHovered = (x >= minus_planet_button->rect.x && x <= minus_planet_button->rect.x + minus_planet_button->rect.w &&
                             y >= minus_planet_button->rect.y && y <= minus_planet_button->rect.y + minus_planet_button->rect.h);
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        // play button
        if (play_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            state = PLAY_INIT;
        }
        // plus planet button
        if (plus_planet_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            num_planets++;
        }
        // minus planet button
        if (minus_planet_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            num_planets--;
        }
    }
}

Button *createButton(int x, int y, int w, int h, const char *text, SDL_Color color) {
    Button *button = (Button *)malloc(sizeof(Button));
    if (!button) {
        return NULL;
    }
    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = w;
    button->rect.h = h;
    button->text = text;
    button->color = color;
    button->isHovered = SDL_FALSE;
    return button;
}