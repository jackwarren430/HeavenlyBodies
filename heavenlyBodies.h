
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h> 

#define SUCCESS 1
#define FAILURE 0
#define FRAMERATE 60

#define PI 3.141f
#define G 1000.0f

typedef struct star {
    char *name;
    float coordinates[2];
    float velocity[2];
    float radius;
    float mass;
} Star;

typedef struct planet {
    char *name;
    float coordinates[2];
    float velocity[2];
    float radius;
    float mass;
} World;
/**
 * life += water * life * ((sun_em_field_strength / distance^2) - planet_em_field_strength)
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

typedef struct SystemPlayback {
    SolarSystem **system_frames;
    float *time_frames;
    int num_frames;
    int frame_capacity;
} SystemPlayback;

World *createPlanet(char *name, float x_coord, float y_coord, float x_vel, float y_vel,float radius, float mass);
Star *createStar(char *name, float x_coord, float y_coord, float x_vel, float y_vel,float radius, float mass);
SolarSystem *createHomogenousSolarSystem(int num_planets, float interval, int distance, int sun_mass, int planet_mass_mult);
SolarSystem *createChaoticSolarSystem(int num_planets, float interval, int distance, int sun_mass, int planet_mass_mult);
SolarSystem *createMoonSystem(int num_planets, float interval, int distance, int sun_mass, int planet_mass_mult);

SystemPlayback *initializePlayback(SolarSystem *system);
int updatePlayback(SolarSystem *system, SystemPlayback *playback, float cur_time);
void deepCopySystem(SolarSystem *dst_system, SolarSystem *src_system);



