#include "physics.h"

void updateSystemPhysics(SolarSystem *system, float delta_time) {
    if (system->sun) {
        calculateSunTotalForce(system, system->sun, delta_time);
        updateSunPosition(system->sun, delta_time);
    }
    for (int i = 0; i < system->num_planets; i++) {
        World *cur_planet = system->planets[i];
        calculateTotalForce(system, cur_planet, delta_time);
        updatePlanetPosition(cur_planet, delta_time);
    }
}

/**
 * 
 *  These functions update the velocity based on force
 * 
 * */

void calculateTotalForce(SolarSystem *system, World *planet, float delta_time) {
    float total_force[2] = {0, 0};
    float distance;
    float mass1;
    float mass2;
    float force_on_planet_x;
    float force_on_planet_y;
    // force from sun
    if (system->sun) {
        distance = calculateDistance(planet->coordinates, system->sun->coordinates);
        mass1 = planet->mass;
        mass2 = system->sun->mass;
        force_on_planet_x = G * ((mass1 * mass2) / pow(distance, 3)) * (system->sun->coordinates[0] - planet->coordinates[0]);
        force_on_planet_y = G * ((mass1 * mass2) / pow(distance, 3)) * (system->sun->coordinates[1] - planet->coordinates[1]);
        total_force[0] += force_on_planet_x;
        total_force[1] += force_on_planet_y;

        // bounce back
        if (distance < planet->radius + system->sun->radius) {
            float vector_x = system->sun->coordinates[0] - planet->coordinates[0];
            float vector_y = system->sun->coordinates[1] - planet->coordinates[1];
            //float theta = atan(vector_y / vector_x);
            float v_dot_v = (vector_x * vector_x) + (vector_y * vector_y);
            float u_dot_v = (vector_x * planet->velocity[0]) + (vector_y * planet->velocity[1]);
            planet->velocity[0] -= (u_dot_v / v_dot_v) * vector_x;
            planet->velocity[1] -= (u_dot_v / v_dot_v) * vector_y;
            system->sun->velocity[0] += ((u_dot_v / v_dot_v) * vector_x) / system->sun->mass;
            system->sun->velocity[1] += ((u_dot_v / v_dot_v) * vector_y) / system->sun->mass;
            total_force[0] -= force_on_planet_x;
            total_force[1] -= force_on_planet_y;
            system->sun->radius += planet->radius / planet->mass;
            system->sun->mass += planet->mass;
            planet->radius = 0;
            planet->mass = 0;
            for (int i = 0; i < system->num_planets; i++) {
                World *cur_planet = system->planets[i];
                if (strcmp(cur_planet->name, planet->name) == 0) {
                    for (int j = i; j < system->num_planets - 1; j++) {
                        system->planets[j] = system->planets[j+1];
                    }
                    system->num_planets--;
                    break;
                }
            }
        }
    }

    
    
    

    // force from planets
    for (int i = 0; i < system->num_planets; i++) {
        World *cur_planet = system->planets[i];
        if (strcmp(cur_planet->name, planet->name) == 0) {
            continue;
        }
        distance = calculateDistance(planet->coordinates, cur_planet->coordinates);
        mass1 = planet->mass;
        mass2 = cur_planet->mass;
        force_on_planet_x = G * ((mass1 * mass2) / pow(distance, 3)) * (cur_planet->coordinates[0] - planet->coordinates[0]);
        force_on_planet_y = G * ((mass1 * mass2) / pow(distance, 3)) * (cur_planet->coordinates[1] - planet->coordinates[1]);
        total_force[0] += force_on_planet_x;
        total_force[1] += force_on_planet_y;
        float collision_loss = 1.0f;
        
        if (distance < planet->radius + cur_planet->radius) {
            float vector_x = cur_planet->coordinates[0] - planet->coordinates[0];
            float vector_y = cur_planet->coordinates[1] - planet->coordinates[1];
            float v_dot_v = (vector_x * vector_x) + (vector_y * vector_y);
            float u_dot_v = (vector_x * planet->velocity[0]) + (vector_y * planet->velocity[1]);
            planet->velocity[0] -= ((u_dot_v / v_dot_v) * vector_x) / planet->mass * collision_loss;
            planet->velocity[1] -= ((u_dot_v / v_dot_v) * vector_y) / planet->mass * collision_loss;
            total_force[0] -= force_on_planet_x;
            total_force[1] -= force_on_planet_y;
            cur_planet->velocity[0] += ((u_dot_v / v_dot_v) * vector_x) / cur_planet->mass * collision_loss;
            cur_planet->velocity[1] += ((u_dot_v / v_dot_v) * vector_y) / cur_planet->mass * collision_loss;
        } 
    }
    float acceleration_x = total_force[0] / planet->mass;
    float acceleration_y = total_force[1] / planet->mass;
    planet->velocity[0] += acceleration_x * delta_time;
    planet->velocity[1] += acceleration_y * delta_time;
}

void calculateSunTotalForce(SolarSystem *system, Star *sun, float delta_time) {
    float total_force[2] = {0, 0};
    for (int i = 0; i < system->num_planets; i++) {
        World *cur_planet = system->planets[i];
        float distance = calculateDistance(sun->coordinates, cur_planet->coordinates);
        float mass1 = sun->mass;
        float mass2 = cur_planet->mass;
        float force_on_sun_x = G * ((mass1 * mass2) / pow(distance, 3)) * (cur_planet->coordinates[0] - sun->coordinates[0]);
        float force_on_sun_y = G * ((mass1 * mass2) / pow(distance, 3)) * (cur_planet->coordinates[1] - sun->coordinates[1]);
        total_force[0] += force_on_sun_x;
        total_force[1] += force_on_sun_y;
        if (distance < cur_planet->radius + sun->radius) {
            float vector_x = cur_planet->coordinates[0] - sun->coordinates[0];
            float vector_y = cur_planet->coordinates[1] - sun->coordinates[1];
            float v_dot_v = (vector_x * vector_x) + (vector_y * vector_y);
            float u_dot_v = (vector_x * sun->velocity[0]) + (vector_y * sun->velocity[1]);
            sun->velocity[0] -= (u_dot_v / v_dot_v) * vector_x;
            sun->velocity[1] -= (u_dot_v / v_dot_v) * vector_y;
            total_force[0] -= force_on_sun_x;
            total_force[1] -= force_on_sun_y;
            cur_planet->velocity[0] += ((u_dot_v / v_dot_v) * vector_x) / cur_planet->mass;
            cur_planet->velocity[1] += ((u_dot_v / v_dot_v) * vector_y) / cur_planet->mass;
        } 

    }
    float acceleration_x = total_force[0] / sun->mass;
    float acceleration_y = total_force[1] / sun->mass;
    sun->velocity[0] += acceleration_x * delta_time;
    sun->velocity[1] += acceleration_y * delta_time;
    
}

/**
 * 
 *  These functions update the position based on velocity
 * 
 * */

void updatePlanetPosition(World *planet, float delta_time) {
    planet->coordinates[0] += planet->velocity[0] * delta_time;
    planet->coordinates[1] += planet->velocity[1] * delta_time;
    //printf("vel: %f\nmult: %f\n\n", planet->velocity[0], planet->velocity[0] * delta_time);
}

void updateSunPosition(Star *sun, float delta_time) {
    sun->coordinates[0] += sun->velocity[0] * delta_time;
    sun->coordinates[1] += sun->velocity[1] * delta_time;
}

float calculateDistance(float coords1[2], float coords2[2]) {
    float x1 = coords1[0];
    float y1 = coords1[1];
    float x2 = coords2[0];
    float y2 = coords2[1];
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

