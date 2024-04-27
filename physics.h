#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

#define FRAMERATE 60
#define G 1000.0f

void updateSystemPhysics(SolarSystem *system, float delta_time);
void calculateTotalForce(SolarSystem *system, World *planet, float delta_time);
void calculateSunTotalForce(SolarSystem *system, Star *sun, float delta_time);
void updatePlanetPosition(World *planet, float delta_time);
void updateSunPosition(Star *sun, float delta_time);
float calculateDistance(float coords1[2], float coords2[2]);