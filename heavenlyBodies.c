
#include "heavenlyBodies.h"


void updateSystemLife(SolarSystem *solar_system, float deltaTime) {

}

void calculateLifeEnergy(World *planet) {

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

SolarSystem *createHomogenousSolarSystem(int num_planets, float interval) {
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

    float radius = 15;
    float mass = 1500.0f;
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
        float theta = i * interval;
        if (theta >= 2 * PI) {
            theta -= 2 * PI;
        }
        x_coord = (i*40 + 50)*cos(theta);
        y_coord = (i*40 + 50)*sin(theta);
        float x_vel = 0.0;
        float y_vel = 0.0;

        float velocity_constant = 160;
        float velocity_offset = 70;
        int i_multiplier = 15;

        if (theta > 0 && theta < PI / 2) {
            x_vel = -(velocity_constant/(i*i_multiplier+1) + velocity_offset)*sin(theta);
            y_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*cos(theta);
        } else if (theta > PI / 2 && theta < PI){
            x_vel = -(velocity_constant/(i*i_multiplier+1) + velocity_offset)*sin(theta);
            y_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*cos(theta);
        } else if (theta > PI && theta < 3 * PI / 2) {
            x_vel = -(velocity_constant/(i*i_multiplier+1) + velocity_offset)*sin(theta);
            y_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*cos(theta);
        } else if (theta > 3 * PI / 2 && theta < 2 * PI) {
            x_vel = -(velocity_constant/(i*i_multiplier+1) + velocity_offset)*sin(theta);
            y_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*cos(theta);
        } else if (theta == 0) {
            x_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*sin(theta);
            y_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*cos(theta);
        } else if (theta == PI / 2) {
            x_vel = -(velocity_constant/(i*i_multiplier+1) + velocity_offset)*sin(theta);
            y_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*cos(theta);
        } else if (theta == PI) {
            x_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*sin(theta);
            y_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*cos(theta);
        } else if (theta == 3 * PI / 2) {
            x_vel = -(velocity_constant/(i*i_multiplier+1) + velocity_offset)*sin(theta);
            y_vel = (velocity_constant/(i*i_multiplier+1) + velocity_offset)*cos(theta);
        }
        

        float radius = 5;
        float mass = 1.0 + (i * 10);
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

SolarSystem *createTwoBodySystem() {
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
    y_vel = 100;

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

SolarSystem *createTwoPlanetSystem() {
    SolarSystem *system = (SolarSystem *)malloc(sizeof(SolarSystem));
    if (!system) {
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }

    system->sun = NULL;

    //Create planets
    World **planets = (World **)malloc(2 * sizeof(World*));
    if (!planets) {
        free(system);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    
    char new_planet_name[15];
    sprintf(new_planet_name, "planet_%d", 1);

    float x_coord = 50;
    float y_coord = 0;
    float x_vel = 0;
    float y_vel = 50;

    float radius = 5;
    float mass = 300.0f;
    World *new_planet = createPlanet(new_planet_name, x_coord, y_coord, x_vel, y_vel, radius, mass, NULL, 0, 0);
    if (!new_planet) {
        free(system);
        free(planets);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    planets[0] = new_planet;

    char new_planet_name2[15];
    sprintf(new_planet_name2, "planet_%d", 2);

    x_coord = -50;
    y_coord = 0;
    x_vel = 0;
    y_vel = -30;

    radius = 5;
    mass = 500.0f;
    World *new_planet2 = createPlanet(new_planet_name2, x_coord, y_coord, x_vel, y_vel, radius, mass, NULL, 0, 0);
    if (!new_planet2) {
        free(system);
        free(planets[0]);
        free(planets);
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }
    planets[1] = new_planet2;

    system->planets = planets;
    system->num_planets = 2;
    return system;
}






