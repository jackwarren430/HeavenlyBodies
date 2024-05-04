
#include "heavenlyBodies.h"


void updateSystemLife(SolarSystem *solar_system, float deltaTime) {

}

void calculateLifeEnergy(World *planet) {

}


SystemPlayback *initializePlayback(SolarSystem *system) {
    SystemPlayback *playback = (SystemPlayback *)malloc(sizeof(SystemPlayback));
    if (!playback) {
        return NULL;
    }
    playback->time_frames = (float*)malloc(100 * sizeof(float));
    if (!playback->time_frames) {
        free(playback);
        return NULL;
    }
    playback->system_frames = (SolarSystem**)malloc(100 * sizeof(SolarSystem*));
    if (!playback->system_frames) {
        free(playback->time_frames);
        free(playback);
        return NULL;
    }
    playback->num_frames = 0;
    playback->frame_capacity = 100;
    return playback;
}


int updatePlayback(SolarSystem *system, SystemPlayback *playback, float cur_time) {
    if (playback->num_frames >= playback->frame_capacity) {
        playback->frame_capacity += 100;
        SolarSystem **new_system_frames = (SolarSystem**)realloc(playback->system_frames, playback->frame_capacity * sizeof(SolarSystem*));
        if (!new_system_frames) {
            return FAILURE;
        }
        playback->system_frames = new_system_frames;
        float *new_time_frames = (float*)realloc(playback->time_frames, playback->frame_capacity * sizeof(float*));
        if (!new_time_frames) {
            free(new_system_frames);
            return FAILURE;
        }
        playback->time_frames = new_time_frames;
    }
    SolarSystem *new_system_frame = (SolarSystem*)malloc(sizeof(SolarSystem));
    if (!new_system_frame) {
        return FAILURE;
    }
    deepCopySystem(new_system_frame, system);
    playback->system_frames[playback->num_frames] = new_system_frame;
    playback->time_frames[playback->num_frames] = cur_time;

    playback->num_frames++;
    return SUCCESS;
}

void deepCopySystem(SolarSystem *dst_system, SolarSystem *src_system) {
    Star *new_star = createStar(src_system->sun->name, src_system->sun->coordinates[0], src_system->sun->coordinates[1],
        src_system->sun->velocity[0], src_system->sun->velocity[1], src_system->sun->radius, src_system->sun->mass, &src_system->sun->em_field_strength);
    if (!new_star) {
        return;
    }
    dst_system->sun = new_star;
    World **new_planets = (World **)malloc(src_system->num_planets * sizeof(World*));
    if (!new_planets) {
        return;
    }
    for (int i = 0; i < src_system->num_planets; i++) {
        World *src_planet = src_system->planets[i];
        World *new_planet = createPlanet(src_planet->name, src_planet->coordinates[0], src_planet->coordinates[1],
            src_planet->velocity[0], src_planet->velocity[1], src_planet->radius, src_planet->mass, &src_planet->em_field_strength, src_planet->water, src_planet->life);
        if (!new_planet) {
            return;
        }
        new_planets[i] = new_planet;
    }
    dst_system->planets = new_planets;
    dst_system->num_planets = src_system->num_planets;
}


World *createPlanet(char *name, float x_coord, float y_coord, float x_vel, float y_vel, float radius, float mass, float *em_field_strength, int water, int life) {
    if (!name) {
        return NULL;
    }
    World *planet_x = (World *)malloc(sizeof(World));
    if (!planet_x) {
        return NULL;
    }
    planet_x->name = (char *)malloc(strlen(name) + 1);
    if (!planet_x->name) {
        free(planet_x);
        return NULL;
    }
    strcpy(planet_x->name, name);
    planet_x->coordinates[0] = x_coord;
    planet_x->coordinates[1] = y_coord;
    planet_x->velocity[0] = x_vel;
    planet_x->velocity[1] = y_vel;
    planet_x->radius = radius;
    planet_x->mass = mass;
    if (em_field_strength) {
        planet_x->em_field_strength = *em_field_strength;
    } else {
        planet_x->em_field_strength = 1;
    }
    planet_x->water = water;
    planet_x->life = life;
    return planet_x;
}

Star *createStar(char *name, float x_coord, float y_coord, float x_vel, float y_vel, float radius, float mass, float *em_field_strength) {
    if (!name) {
        return NULL;
    }
    Star *star_x = (Star *)malloc(sizeof(Star));
    if (!star_x) {
        return NULL;
    }
    star_x->name = (char *)malloc(strlen(name) + 1);
    if (!star_x->name) {
        free(star_x);
        return NULL;
    }
    strcpy(star_x->name, name);
    star_x->coordinates[0] = x_coord;
    star_x->coordinates[1] = y_coord;
    star_x->velocity[0] = x_vel;
    star_x->velocity[1] = y_vel;
    star_x->radius = radius;
    star_x->mass = mass;
    if (em_field_strength) {
        star_x->em_field_strength = *em_field_strength;
    } else {
        star_x->em_field_strength = 1;
    }
    return star_x;
}

/**
 * 
 *  Creates a Simple Solar System containing a central Sun, and numPlanet orbiting planets
 * 
 **/

SolarSystem *createHomogenousSolarSystem(int num_planets, float interval, int distance, int sun_mass, int planet_mass_mult) {
    SolarSystem *system = (SolarSystem *)malloc(sizeof(SolarSystem));
    if (!system) {
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }

    // Create Sun
    char *sun_name = "Helius";

    float x_coord = 0.0f;
    float y_coord = 0.0f;
    float x_vel = 0.0f;
    float y_vel = 0.0f;

    
    float mass = (float)sun_mass;
    float radius = sun_mass * 0.005f;
    Star *sun = createStar(sun_name, x_coord, y_coord, x_vel, y_vel, radius, mass, NULL);
    if (!sun) {
        free(system);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    system->sun = sun;

    //Create planets
    World **planets = (World **)malloc(num_planets * sizeof(World*));
    if (!planets) {
        free(system->sun->name);
        free(system->sun);
        free(system);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    distance += radius;
    for (int i = 0; i < num_planets; i++) {
        char new_planet_name[15];
        sprintf(new_planet_name, "planet_%d", i);
        float theta = i * interval;
        while (theta >= 2 * PI) {
            theta -= 2 * PI;
        }
        x_coord = ((i+1)*distance)*cos(theta);
        y_coord = ((i+1)*distance)*sin(theta);

        float distance_c = sqrt(pow(x_coord, 2) + pow(y_coord, 2));
        float x_vel = 0.0;
        float y_vel = 0.0;
        float offset = 0;
        float velocity = sqrt(G * system->sun->mass / distance_c) + offset;

        x_vel = velocity*sin(theta);
        y_vel = velocity*cos(theta);

        

        if (theta != 0 && theta != PI) {
            x_vel *= -1;
        } else {
            char *new_planet_target = "target";
            strcpy(new_planet_name, new_planet_target);
        }
        
        
        float mass = planet_mass_mult * (i + 1);
        float radius = mass * 0.1f + 10;
        World *new_planet = createPlanet(new_planet_name, x_coord, y_coord, x_vel, y_vel, radius, mass, NULL, 0, 0);
        if (!new_planet) {
            free(system->sun->name);
            free(system->sun);
            free(system);
            for (int j = i - 1; j >= 0; j--) {
                free(planets[j]->name);
                free(planets[j]);
            }
            free(planets);
            printf("Failed to create SolarSystem: malloc error\n");
            return NULL;
        }
        planets[i] = new_planet;
    }
    system->planets = planets;
    system->num_planets = num_planets;
    return system;
}

SolarSystem *createMoonSystem() {
    SolarSystem *system = (SolarSystem *)malloc(sizeof(SolarSystem));
    if (!system) {
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }

    // Create Sun
    char *sun_name = "Helius";

    int x_coord = 0;
    int y_coord = 0;
    float x_vel = 0;
    float y_vel = 0;

    float radius = 10;
    float mass = 1000.0f;
    Star *sun = createStar(sun_name, x_coord, y_coord, x_vel, y_vel, radius, mass, NULL);
    if (!sun) {
        free(system);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    system->sun = sun;

    //Create planets
    World **planets = (World **)malloc(sizeof(World*));
    if (!planets) {
        free(system->sun->name);
        free(system->sun);
        free(system);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    
    char new_planet_name[15];
    sprintf(new_planet_name, "planet_%d", 1);

    x_coord = 100;
    y_coord = 0;
    x_vel = 0;
    y_vel = 0;

    radius = 5;
    mass = 1.0f;
    World *new_planet = createPlanet(new_planet_name, x_coord, y_coord, x_vel, y_vel, radius, mass, NULL, 0, 0);
    if (!new_planet) {
        free(system->sun->name);
        free(system->sun);
        free(system);
        free(planets);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    planets[0] = new_planet;
    system->planets = planets;
    system->num_planets = 1;
    return system;
}



SolarSystem *createChaoticSolarSystem(int num_planets, float interval, int distance, int sun_mass, int planet_mass_mult) {
    SolarSystem *system = (SolarSystem *)malloc(sizeof(SolarSystem));
    if (!system) {
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }

    // Create Sun
    char *sun_name = "Helius";

    float x_coord = 0.0f;
    float y_coord = 0.0f;
    float x_vel = 0.0f;
    float y_vel = 0.0f;

    
    float mass = (float)sun_mass;
    float radius = sun_mass * 0.005f;
    Star *sun = createStar(sun_name, x_coord, y_coord, x_vel, y_vel, radius, mass, NULL);
    if (!sun) {
        free(system);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    system->sun = sun;

    //Create planets
    World **planets = (World **)malloc(num_planets * sizeof(World*));
    if (!planets) {
        free(system->sun->name);
        free(system->sun);
        free(system);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    
    for (int i = 0; i < num_planets; i++) {
        char new_planet_name[15];
        sprintf(new_planet_name, "planet_%d", i);

        int r = rand() % (60);
        float theta = r * PI / 30;
        while (theta >= 2 * PI) {
            theta -= 2 * PI;
        }
        x_coord = ((i+1)*distance)*cos(theta);
        y_coord = ((i+1)*distance)*sin(theta);
        r = rand() % (60);
        theta = r * PI / 30;
        while (theta >= 2 * PI) {
            theta -= 2 * PI;
        }
        float x_coord2 = pow(-1, i)*((i+1)*distance)*cos(theta);
        float y_coord2 = pow(-1, i)*((i+1)*distance)*sin(theta);
        float distance_c = pow(x_coord2, 2) + pow(y_coord2, 2);
        float x_vel = 0.0;
        float y_vel = 0.0;
        float offset = 0.0001;
        float velocity = sqrt(G * system->sun->mass / (distance_c * offset));

        x_vel = pow(-1, i) * velocity * sin(theta);
        y_vel = pow(-1, i) * velocity * cos(theta);

        

        if (theta != 0 && theta != PI) {
            x_vel *= -1;
        } else {
            char *new_planet_target = "target";
            strcpy(new_planet_name, new_planet_target);
        }
        
        
        float mass = planet_mass_mult * (i + 1);
        float radius = mass * 0.1f + 10;
        World *new_planet = createPlanet(new_planet_name, x_coord, y_coord, x_vel, y_vel, radius, mass, NULL, 0, 0);
        if (!new_planet) {
            free(system->sun->name);
            free(system->sun);
            free(system);
            for (int j = i - 1; j >= 0; j--) {
                free(planets[j]->name);
                free(planets[j]);
            }
            free(planets);
            printf("Failed to create SolarSystem: malloc error\n");
            return NULL;
        }
        planets[i] = new_planet;
    }
    system->planets = planets;
    system->num_planets = num_planets;
    return system;
}




