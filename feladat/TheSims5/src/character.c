#include "character.h"
//#include "app.h"  // a teljes App típus miatt

void update_character(Character* player, float delta_time, App* app)
{
    const float gravity = -9.81f;
    const float terminal_velocity = -50.0f;

    if (!player->is_on_ground) {
        player->velocity.y += gravity * delta_time;
        if (player->velocity.y < terminal_velocity) {
            player->velocity.y = terminal_velocity;
        }
    }

    // Pozíció frissítés
    player->position.x += player->velocity.x * delta_time;
    player->position.y += player->velocity.y * delta_time;
    player->position.z += player->velocity.z * delta_time;

    // Egyszerű talajérzékelés
    if (player->position.y <= 0.0f) {
        player->position.y = 0.0f;
        player->velocity.y = 0.0f;
        player->is_on_ground = true;
    }

    printf("player pos X: %d\n", player->position.x);

}

void init_character(Character* player)
{
    player->position.x = 0.0f;
    player->position.y = 0.0f;
    player->position.z = 0.0f;

    player->velocity.x = 0.0f;
    player->velocity.y = 0.0f;
    player->velocity.z = 0.0f;

    player->is_on_ground = true;
}