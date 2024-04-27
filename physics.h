#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

void updateSystemPhysics(SolarSystem *system);
void calculateTotalForce(SolarSystem *system, World *planet);
void calculateSunTotalForce(SolarSystem *system, Star *sun);
void updatePlanetPosition(World *planet);
void updateSunPosition(Star *sun);