#include <stdio.h>
#include <stdlib.h>

#include "island.h"
#include "rng.h"
#include "rabbit.h"

#include "rabbit_logic.h"
#include "wolf_logic.h"

#include "params.h"

Island* engine_init(int seed) {
    printf("Engine init\n");

    // Initialize random number generator
    rng_init(seed);

    // Initialize island
    Island* island = malloc(sizeof(Island));
    island->day = 0;

    // Initialize animals
    island->first_rabbit = generate_n_random_rabbits(K);

    island->first_wolf = generate_n_random_wolves(70); // TODO: verify


    return island;
}

void engine_tick(Island* island) {
    island->day++;

    // Tick rabbits

    // Check if there are any rabbits left
    if (island->first_rabbit == NULL) {
        printf("All rabbits have died\n");
        return;
    }

    // First rabbit need to be handled separately, so we can update the first_rabbit pointer if it dies
    bool has_survived = rabbit_tick(island->first_rabbit, island->day);

    if (!has_survived) {
        Rabbit* next_rabbit = island->first_rabbit->next;
        //free(island->first_rabbit);
        // First rabbit has died, update the first_rabbit pointer
        island->first_rabbit = next_rabbit;
    }

    Rabbit* current_rabbit = has_survived ? island->first_rabbit->next : island->first_rabbit;
    while (current_rabbit != NULL) {
        rabbit_tick(current_rabbit, island->day);
        current_rabbit = current_rabbit->next;
    }

    // Tick wolves

    // Check if there are any wolves left
    if (island->first_wolf == NULL) {
        printf("All wolves have died\n");
        return;
    }

    // First wolf need to be handled separately, so we can update the first_wolf pointer if it dies
    has_survived = wolf_tick(island->first_wolf, island->day, island->first_rabbit);

    if (!has_survived) {
        Wolf* next_wolf = island->first_wolf->next;
        //free(island->first_wolf);
        // First wolf has died, update the first_wolf pointer
        island->first_wolf = next_wolf;
    }

    Wolf* current_wolf = has_survived ? island->first_wolf->next : island->first_wolf;
    while (current_wolf != NULL) {
        wolf_tick(current_wolf, island->day, island->first_rabbit);
        current_wolf = current_wolf->next;
    }

    // Iterate through all wolves and if ->died, remove them from the list
    current_wolf = island->first_wolf;
    while (current_wolf != NULL) {
        if (current_wolf->died) {
            Wolf* next_wolf = current_wolf->next;
            remove_wolf(current_wolf);
            current_wolf = next_wolf;
        } else {
            current_wolf = current_wolf->next;
        }
    }

    // Breeding tick (rabbits)
    current_rabbit = island->first_rabbit;
    while (current_rabbit != NULL) {
        rabbit_breeding_tick(current_rabbit, island->first_rabbit);
        current_rabbit = current_rabbit->next;
    }

    // Breeding tick (wolves)
    current_wolf = island->first_wolf;
    while (current_wolf != NULL) {
        wolf_breeding_tick(current_wolf, island->first_wolf);
        current_wolf = current_wolf->next;
    }
    
}

void engine_deinit(Island* island) {
    Rabbit* current_rabbit = island->first_rabbit;
    while (current_rabbit != NULL) {
        Rabbit* next_rabbit = current_rabbit->next;
        free(current_rabbit);
        current_rabbit = next_rabbit;
    }

    free(island);

    printf("Engine deinit\n");
}