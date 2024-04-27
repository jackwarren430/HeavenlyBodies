
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SUCCESS 1
#define FAILURE 0

#define PI 3.141

typedef struct star {
    char *name;
    int coordinates[2];
    int velocity[2];
    float radius;
    float mass;
    float em_field_strength;
} Star;

typedef struct planet {
    char *name;
    int coordinates[2];
    int velocity[2];
    float radius;
    float mass;
    float em_field_strength;
    float water;
    int life; // life += water * life * ((sun_em_field_strength / distance^2) - planet_em_field_strength)
} World;
/**
 * 
 * life represents the total amount of sustained life on a planet
 * water represents the necessary ingredients nescesarry for life
 * 
 * this equation calculates the amount of energy entering into the
 * planet system, which is needed for life:
 * 
 *   --need to make em_field_strenght a function of mass and radius
 * 
 * ((sun_em_field_strength / distance^2) - planet_em_field_strength)
 * 
 * // if the sun is too strong, 
 * 
 * */

typedef struct system {
    Star *sun;
    World **planets;
    int num_planets;
} SolarSystem;

World *createPlanet(char *name, int x_coord, int y_coord, float radius, float mass, float *em_field_strength, int water, int life);
Star *createStar(char *name, int x_coord, int y_coord, float radius, float mass, float *em_field_strength);
SolarSystem *createHomogenousSolarSystem(int num_planets);
void updateSystemLife(SolarSystem *solar_system);
void calculateLifeEnergy(World *planet);
