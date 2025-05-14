#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>

#include "utils.h"

#include <stdbool.h>


typedef struct Character {
    vec3 position;     // karakter pozíciója (x, y, z)
    vec3 velocity;     // sebesség (pl. ugrás, esés)
    float mass;        // tömeg (gravitációhoz hasznos lehet)
    bool is_on_ground; // talajon van-e
} Character;

void update_character(Character* player, float delta_time, App* app);

void init_character(Character* player);

#endif