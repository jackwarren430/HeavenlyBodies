#include "physics.h"

void updateSystemPhysics(SolarSystem *system) {
    calculateSunTotalForce(system, system->sun);
    updateSunPosition(system->sun);
    for (int i = 0; i < system->num_planets; i++) {
        World *cur_planet = system->planets[i];
        calculateTotalForce(system, cur_planet);
        updatePlanetPosition(cur_planet);
    }
}

void calculateTotalForce(SolarSystem *system, World *planet) {
    float acceleration[2] = [0, 0];
    for (int i = 0; i < system->num_planets; i++) {
        World *cur_planet = system->planets[i];
        
    }
}

void calculateSunTotalForce(SolarSystem *system, Star *sun) {
    
}

void updatePlanetPosition(World *planet) {

}

void updateSunPosition(Star *sun) {

}

