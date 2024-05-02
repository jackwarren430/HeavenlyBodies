#include "ui.h"

State state;
extern SDL_bool done;
extern int view_width;

// menu stuff
Button *play_button;
Button *plus_planet_button;
Button *minus_planet_button;
int num_planets;

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
        view_width = SCREEN_WIDTH;
        num_planets = 1;
        SDL_Color color = (SDL_Color){255, 255, 255, 255};
        play_button = createButton(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 40, 80, 40, "Play", color);
        plus_planet_button = createButton(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2 + 30, 30, 30, "+", color);
        minus_planet_button = createButton(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 30, 30, 30, "-", color);
        state = MENU;
        break;}
    case MENU: {
        renderMenu(renderer, font, font_big, play_button, plus_planet_button, minus_planet_button, num_planets);
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
        handlePlayButtonEvent(event);
        break;}
    case PLAYBACK_INIT: break;
    case PLAYBACK: {
        handlePlaybackButtonEvent(event);
        break;}
    case PLAYBACK_PAUSE_INIT: break;
    case PLAYBACK_PAUSE: {
        handlePauseButtonEvent(event);
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
        if (minus_planet_button->isHovered && event->button.button == SDL_BUTTON_LEFT && num_planets > 1) {
            num_planets--;
        }

    }
}

void handlePlayButtonEvent(SDL_Event *event) {
    if (event->type == SDL_MOUSEMOTION) {
        int x = event->motion.x;
        int y = event->motion.y;
        // end button
        end_button->isHovered = (x >= end_button->rect.x && x <= end_button->rect.x + end_button->rect.w &&
                             y >= end_button->rect.y && y <= end_button->rect.y + end_button->rect.h);

        zoom_out_button->isHovered = (x >= zoom_out_button->rect.x && x <= zoom_out_button->rect.x + zoom_out_button->rect.w &&
                             y >= zoom_out_button->rect.y && y <= zoom_out_button->rect.y + zoom_out_button->rect.h);
    
        zoom_in_button->isHovered = (x >= zoom_in_button->rect.x && x <= zoom_in_button->rect.x + zoom_in_button->rect.w &&
                             y >= zoom_in_button->rect.y && y <= zoom_in_button->rect.y + zoom_in_button->rect.h);
    
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
        int x = event->motion.x;
        int y = event->motion.y;
        // pause button
        pause_button->isHovered = (x >= pause_button->rect.x && x <= pause_button->rect.x + pause_button->rect.w &&
                             y >= pause_button->rect.y && y <= pause_button->rect.y + pause_button->rect.h);
    
        menu_button->isHovered = (x >= menu_button->rect.x && x <= menu_button->rect.x + menu_button->rect.w &&
                             y >= menu_button->rect.y && y <= menu_button->rect.y + menu_button->rect.h);
    
        zoom_out_button->isHovered = (x >= zoom_out_button->rect.x && x <= zoom_out_button->rect.x + zoom_out_button->rect.w &&
                             y >= zoom_out_button->rect.y && y <= zoom_out_button->rect.y + zoom_out_button->rect.h);
    
        zoom_in_button->isHovered = (x >= zoom_in_button->rect.x && x <= zoom_in_button->rect.x + zoom_in_button->rect.w &&
                             y >= zoom_in_button->rect.y && y <= zoom_in_button->rect.y + zoom_in_button->rect.h);
    
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {

        // pause button
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
        int x = event->motion.x;
        int y = event->motion.y;
        // play button
        play_button->isHovered = (x >= play_button->rect.x && x <= play_button->rect.x + play_button->rect.w &&
                             y >= play_button->rect.y && y <= play_button->rect.y + play_button->rect.h);
        // plus button
        plus_frame_button->isHovered = (x >= plus_frame_button->rect.x && x <= plus_frame_button->rect.x + plus_frame_button->rect.w &&
                             y >= plus_frame_button->rect.y && y <= plus_frame_button->rect.y + plus_frame_button->rect.h);
        // minus button
        minus_frame_button->isHovered = (x >= minus_frame_button->rect.x && x <= minus_frame_button->rect.x + minus_frame_button->rect.w &&
                             y >= minus_frame_button->rect.y && y <= minus_frame_button->rect.y + minus_frame_button->rect.h);
        
        menu_button->isHovered = (x >= menu_button->rect.x && x <= menu_button->rect.x + menu_button->rect.w &&
                             y >= menu_button->rect.y && y <= menu_button->rect.y + menu_button->rect.h);
        
        zoom_out_button->isHovered = (x >= zoom_out_button->rect.x && x <= zoom_out_button->rect.x + zoom_out_button->rect.w &&
                             y >= zoom_out_button->rect.y && y <= zoom_out_button->rect.y + zoom_out_button->rect.h);
    
        zoom_in_button->isHovered = (x >= zoom_in_button->rect.x && x <= zoom_in_button->rect.x + zoom_in_button->rect.w &&
                             y >= zoom_in_button->rect.y && y <= zoom_in_button->rect.y + zoom_in_button->rect.h);
    

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