#include "ui.h"

State state;
extern SDL_bool done;
extern int view_width;
extern int x_offset;
extern int y_offset;

// menu stuff
extern Button *play_button;
extern Button *plus_planet_button;
extern Button *minus_planet_button;
extern int num_planets;
extern Button *plus_sun_mass_button;
extern Button *minus_sun_mass_button;
extern int sun_mass;
extern Button *plus_planet_mass_button;
extern Button *minus_planet_mass_button;
extern int planet_mass_mult;
extern Button *plus_distance_button;
extern Button *minus_distance_button;
extern int distance;

// play stuff
Button *end_button;
Button *pause_button;
Button *play_button;
Button *plus_frame_button;
Button *minus_frame_button;
Button *zoom_out_button;
Button *zoom_in_button;
SolarSystem *solar_system;
SystemPlayback *playback;
float total_time;
int cur_frame;

Button *menu_button;

void mainLoop(float deltaTime, SDL_Renderer *renderer, TTF_Font *font, TTF_Font *font_big) {

    switch (state) {
    case MENU_INIT: {
        view_width = 2000;
        srand(time(NULL));
        x_offset = 0;
        y_offset = 0;
        num_planets = 1;
        sun_mass = 5000;
        planet_mass_mult = 1;
        distance = 50;
        SDL_Color color = (SDL_Color){255, 255, 255, 255};
        play_button = createButton(PLAY_BUTTON_X, PLAY_BUTTON_Y, 80, 40, "Play", color);
        plus_planet_button = createButton(PLUS_PLANET_X, SOLAR_SYSTEM_Y, 30, 30, "+", color);
        minus_planet_button = createButton(MINUS_PLANET_X, SOLAR_SYSTEM_Y, 30, 30, "-", color);
        plus_sun_mass_button = createButton(PLUS_SUNMASS_X, SOLAR_SYSTEM_Y, 30, 30, "+", color);
        minus_sun_mass_button = createButton(MINUS_SUNMASS_X, SOLAR_SYSTEM_Y, 30, 30, "-", color);
        plus_planet_mass_button = createButton(PLUS_PLANETMASS_X, SOLAR_SYSTEM_Y, 30, 30, "+", color);
        minus_planet_mass_button = createButton(MINUS_PLANETMASS_X, SOLAR_SYSTEM_Y, 30, 30, "-", color);
        plus_distance_button = createButton(PLUS_DISTANCE_X, SOLAR_SYSTEM_Y, 30, 30, "+", color);
        minus_distance_button = createButton(MINUS_DISTANCE_X, SOLAR_SYSTEM_Y, 30, 30, "-", color);
        state = MENU;
        break;}
    case MENU: {
        renderMenu(renderer);
        
        break;}
    case PLAY_INIT: {
        SDL_Color color = (SDL_Color){255, 255, 255, 255};
        end_button = createButton(70, 60, 50, 35, "End", color);
        zoom_out_button = createButton(70, 120, 100, 35, "zoom out", color);
        zoom_in_button = createButton(200, 120, 100, 35, "zoom in", color);
        solar_system = initializeSystem();
        playback = initializePlayback(solar_system);
        total_time = 0;
        if (!solar_system || !playback) {
            done = SDL_TRUE;
        }
        state = PLAY;
        break;}
    case PLAY: {
        updateSystemPhysics(solar_system, deltaTime);
        updateSystemLife(solar_system, deltaTime);
        total_time += deltaTime;
        updatePlayback(solar_system, playback, total_time);
        renderGraphics(renderer, font, solar_system);
        renderButton(renderer, font, *end_button);
        renderButton(renderer, font, *zoom_out_button);
        renderButton(renderer, font, *zoom_in_button);
        renderTimeFrame(renderer, font, total_time);
        break;}
    case PLAYBACK_INIT: {
        cur_frame = 0;
        SDL_Color color = (SDL_Color){255, 255, 255, 255};
        pause_button = createButton(70, 60, 80, 35, "Pause", color);
        menu_button = createButton(SCREEN_WIDTH - 80, 60, 80, 35, "Menu", color);
        state = PLAYBACK;
        break; }
    case PLAYBACK: {
        renderGraphics(renderer, font, playback->system_frames[cur_frame]);
        renderTimeFrame(renderer, font, playback->time_frames[cur_frame]);
        renderButton(renderer, font, *pause_button);
        renderButton(renderer, font, *menu_button);
        renderButton(renderer, font, *zoom_out_button);
        renderButton(renderer, font, *zoom_in_button);
        if (cur_frame < playback->num_frames - 1) {
            cur_frame++;
        }
        break;}
    case PLAYBACK_PAUSE_INIT: {
        SDL_Color color = (SDL_Color){255, 255, 255, 255};
        play_button = createButton(70, 60, 60, 35, "Play", color);
        plus_frame_button = createButton(145, 62, 30, 30, "+", color);
        minus_frame_button = createButton(30, 62, 30, 30, "-", color);
        menu_button = createButton(SCREEN_WIDTH - 100, 60, 80, 35, "Menu", color);
        state = PLAYBACK_PAUSE;
        break;}
    case PLAYBACK_PAUSE: {
        renderGraphics(renderer, font, playback->system_frames[cur_frame]);
        renderTimeFrame(renderer, font, playback->time_frames[cur_frame]);
        renderButton(renderer, font, *play_button);
        renderButton(renderer, font, *plus_frame_button);
        renderButton(renderer, font, *minus_frame_button);
        renderButton(renderer, font, *menu_button);
        renderButton(renderer, font, *zoom_out_button);
        renderButton(renderer, font, *zoom_in_button);
        break;}
    }
    SDL_RenderPresent(renderer);
}

SolarSystem *initializeSystem() {
    //SolarSystem *solar_system = createHomogenousSolarSystem(num_planets, PI / 3, distance, sun_mass, planet_mass_mult);
    solar_system = createChaoticSolarSystem(num_planets, PI / 3, distance, sun_mass, planet_mass_mult);
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
        handlePlayButtonEvent(event);
        handleKeyboardEvent(event);
        break;}
    case PLAYBACK_INIT: break;
    case PLAYBACK: {
        handlePlaybackButtonEvent(event);
        handleKeyboardEvent(event);
        break;}
    case PLAYBACK_PAUSE_INIT: break;
    case PLAYBACK_PAUSE: {
        handlePauseButtonEvent(event);
        handleKeyboardEvent(event);
        break;}
    }
}

void handleKeyboardEvent(SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
        case SDLK_UP:
            y_offset += 20;
            break;
        case SDLK_DOWN:
            y_offset -= 20;
            break;
        case SDLK_LEFT:
            x_offset += 20;
            break;
        case SDLK_RIGHT:
            x_offset -= 20;
            break;
        }
    }
}

void handleMenuButtonEvent(SDL_Event *event) {
    if (event->type == SDL_MOUSEMOTION) {
        // play button
        buttonIsHovered(play_button, event);
        buttonIsHovered(plus_planet_button, event);
        buttonIsHovered(minus_planet_button, event);
        buttonIsHovered(plus_sun_mass_button, event);
        buttonIsHovered(minus_sun_mass_button, event);
        buttonIsHovered(plus_planet_mass_button, event);
        buttonIsHovered(minus_planet_mass_button, event);
        buttonIsHovered(plus_distance_button, event);
        buttonIsHovered(minus_distance_button, event);

    } else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        // play button
        if (play_button->isHovered) {
            state = PLAY_INIT;
        }
        // plus planet button
        if (plus_planet_button->isHovered) {
            num_planets++;
        }
        // minus planet button
        if (minus_planet_button->isHovered && num_planets > 1) {
            num_planets--;
        }

        if (plus_sun_mass_button->isHovered) {
            sun_mass += 500;
        }

        if (minus_sun_mass_button->isHovered) {
            sun_mass -= 500;
        }

        if (plus_planet_mass_button->isHovered) {
            planet_mass_mult += 1;
        }

        if (minus_planet_mass_button->isHovered) {
            planet_mass_mult -= 1;
        }

        if (plus_distance_button->isHovered) {
            distance += 5;
        }

        if (minus_distance_button->isHovered) {
            distance -= 5;
        }

    }
}

void handlePlayButtonEvent(SDL_Event *event) {
    if (event->type == SDL_MOUSEMOTION) {
        buttonIsHovered(end_button, event);
        buttonIsHovered(zoom_out_button, event);
        buttonIsHovered(zoom_in_button, event);
        
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        // end button
        if (end_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            state = PLAYBACK_INIT;
        }

        if (zoom_out_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
           zoomOut();
        }

        if (zoom_in_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            zoomIn();
        }
        
    }
}

void handlePlaybackButtonEvent(SDL_Event *event) {
    if (event->type == SDL_MOUSEMOTION) {
        buttonIsHovered(pause_button, event);
        buttonIsHovered(menu_button, event);
        buttonIsHovered(zoom_out_button, event);
        buttonIsHovered(zoom_in_button, event);
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {

        
        if (pause_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            state = PLAYBACK_PAUSE_INIT;
        }

        if (menu_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            state = MENU_INIT;
        }

        if (zoom_out_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            zoomOut();
        }

        if (zoom_in_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            zoomIn();
        }
        
    }
}

void handlePauseButtonEvent(SDL_Event *event) {
    if (event->type == SDL_MOUSEMOTION) {
        buttonIsHovered(play_button, event);
        buttonIsHovered(plus_frame_button, event);
        buttonIsHovered(minus_frame_button, event);
        buttonIsHovered(menu_button, event);
        buttonIsHovered(zoom_out_button, event);
        buttonIsHovered(zoom_in_button, event);

    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        // play button
        if (play_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            state = PLAYBACK;
        }

        if (plus_frame_button->isHovered && event->button.button == SDL_BUTTON_LEFT && cur_frame < playback->num_frames - 8) {
            cur_frame += 8;
        }

        if (minus_frame_button->isHovered && event->button.button == SDL_BUTTON_LEFT && cur_frame > 8) {
            cur_frame -= 8;
        }

        if (menu_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            state = MENU_INIT;
        }

        if (zoom_out_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            zoomOut();
        }

        if (zoom_in_button->isHovered && event->button.button == SDL_BUTTON_LEFT) {
            zoomIn();
        }
        
    }
}

void buttonIsHovered(Button *button, SDL_Event *event) {
    int x = event->motion.x;
    int y = event->motion.y;
    button->isHovered = (x >= button->rect.x && x <= button->rect.x + button->rect.w &&
                             y >= button->rect.y && y <= button->rect.y + button->rect.h);
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