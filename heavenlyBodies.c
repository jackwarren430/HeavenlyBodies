
#include "heavenlyBodies.h"


void updateSystemLife(SolarSystem *solar_system) {

}

void calculateLifeEnergy(World *planet) {

}


World *createPlanet(char *name, int x_coord, int y_coord, float radius, float mass, float *em_field_strength, int water, int life) {
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
    planet_x->velocity[0] = 0;
    planet_x->velocity[1] = 0;
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

Star *createStar(char *name, int x_coord, int y_coord, float radius, float mass, float *em_field_strength) {
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
    star_x->velocity[0] = 0;
    star_x->velocity[1] = 0;
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

SolarSystem *createHomogenousSolarSystem(int num_planets) {
    SolarSystem *system = (SolarSystem *)malloc(sizeof(SolarSystem));
    if (!system) {
        printf("Failed to create SolarSystem: malloc error\n");
        return NULL;
    }

    // Create Sun
    char *sun_name = "Helius";
    float radius = 10;
    float mass = 100;
    Star *sun = createStar(sun_name, 0, 0, radius, mass, NULL);
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
        int x_coord = (i*40 + 50)*cos(i * PI / 4);
        int y_coord = (i*40 + 50)*sin(i * PI / 4);
        //printf("x: %d, y: %d\n", x_coord, y_coord);
        float radius = 5;
        float mass = 5.0;
        World *new_planet = createPlanet(new_planet_name, x_coord, y_coord, radius, mass, NULL, 0, 0);
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






