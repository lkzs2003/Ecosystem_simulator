#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "rng.h"
#include "utils.h"
#include "animal.h"
#include "rabbit.h"

#include "rabbit_logic.h"

#include "params.h"

Rabbit* generate_random_rabbit(Rabbit* prev) {
    Rabbit* rabbit = malloc(sizeof(Rabbit));

    rabbit->location_x = rng_rand(1, L);
    rabbit->location_y = rng_rand(1, L);
    rabbit->age = rng_rand(M, S);
    rabbit->gender = rng_rand(0, 1) == 1 ? MALE : FEMALE;
    rabbit->is_pregnant = false;
    rabbit->pregnancy_remaining = 0;
    rabbit->is_adult = true;

    rabbit->prev = prev;
    rabbit->next = NULL;

    return rabbit;
}

Rabbit* generate_random_child(Rabbit* prev, Rabbit* parent) {
    Rabbit* rabbit = malloc(sizeof(Rabbit));

    rabbit->location_x = parent->location_x;
    rabbit->location_y = parent->location_y;
    rabbit->age = 0;
    rabbit->gender = rng_rand(0, 1) == 1 ? MALE : FEMALE;
    rabbit->is_pregnant = false;
    rabbit->pregnancy_remaining = 0;
    rabbit->is_adult = false;

    rabbit->prev = prev;
    rabbit->next = NULL;

    return rabbit;
}

Rabbit* generate_n_random_rabbits(int n) {
    Rabbit* first_rabbit = NULL;
    Rabbit* current_rabbit = NULL;

    for (int i = 0; i < n; i++) {
        if (first_rabbit == NULL) {
            first_rabbit = generate_random_rabbit(NULL);
            current_rabbit = first_rabbit;
        } else {
            current_rabbit->next = generate_random_rabbit(current_rabbit);
            current_rabbit = current_rabbit->next;
        }
    }

    return first_rabbit;
}

// Returns true if the rabbit is still alive, false otherwise
bool rabbit_tick(Rabbit* rabbit, int day) {

    // ==== MOVEMENT ====
    // Every tick a rabbit moves by one unit in a random direction
    // Note that the island has limited size (1-L)*(1-L)

    int delta_x = rng_rand(-10, 10); //TODO: decide on the range
    int delta_y = rng_rand(-10, 10);

    rabbit->location_x += delta_x;
    rabbit->location_y += delta_y;

    // If the rabbit is outside the island, move it back inside
    rabbit->location_x = clamp(1, rabbit->location_x, L);
    rabbit->location_y = clamp(1, rabbit->location_y, L);

    // ==== AGE ====
    // We store age in weeks, so simply increment it by one when day is divisible by 7
    if (day % 7 == 0) {
        rabbit->age++;
    }
    // If the rabbit is older than M weeks, it becomes an adult
    if (rabbit->age > M) {
        rabbit->is_adult = true;
    }

    // ==== DEATH ====
    // If the rabbit is between (S-SD, S+SD) weeks old, it has a 50% chance of dying
    // If it's older than S+SD weeks, it dies for sure
    if (rabbit->age > S + SD || (rabbit->age > S - SD && rng_rand(0, 100) < 51)) {
        remove_rabbit(rabbit);
        return false;
    }

    // ==== PREGNANCY ====
    if (rabbit->gender == FEMALE) {
        // If the rabbit is pregnant, decrement the remaining time (every 7 days)
        if (rabbit->is_pregnant) {
            if (day % 7 == 0) {
                rabbit->pregnancy_remaining--;
            }

            // If the pregnancy is over, give birth to (MINk, MAXk) rabbits (randomly)
            if (rabbit->pregnancy_remaining == 0) {
                int num_offspring = rng_rand(MINk, MAXk);

                // Generate and add the offspring
                for (int i = 0; i < num_offspring; i++) {
                    Rabbit* offspring = generate_random_child(rabbit, rabbit);
                    offspring->is_adult = false;

                    // Add the offspring to the list
                    if (rabbit->prev == NULL) {
                        // If the rabbit is the first one, set it as the first one
                        rabbit->prev = offspring;
                        offspring->next = rabbit;
                    } else {
                        // If the rabbit is not the first one, insert it between the previous and the current rabbit
                        Rabbit* prev_rabbit = (Rabbit*) rabbit->prev;
                        prev_rabbit->next = offspring;
                        offspring->prev = prev_rabbit;
                        offspring->next = rabbit;
                        rabbit->prev = offspring;
                    }

                    // Clear pregnancy flags
                    rabbit->is_pregnant = false;
                    rabbit->pregnancy_remaining = 0;
                }
            }
        }
    }
    return true;
}

void rabbit_breeding_tick(Rabbit* rabbit, Rabbit* first_rabbit) {
    // ==== BREEDING ====
    // If the rabbit is an adult non-pregnant female and is on the same tile as an adult male, it becomes pregnant
    if (rabbit->gender == FEMALE && !rabbit->is_pregnant && rabbit->is_adult) {
        // Iterate over all rabbits
        // If we find one, that:
        // 1. Is within 10 units of us //TODO: decide on the range
        // 2. Is an adult
        // 3. Is a male
        // 4. Is not ourself (which is a redundant check, as we are a female, but why not)
        // Our rabbit becomes pregnant

        int our_x = rabbit->location_x;
        int our_y = rabbit->location_y;

        Rabbit* current_rabbit = first_rabbit;
        while (current_rabbit != NULL) {
            if (abs(current_rabbit->location_x - our_x) <= 10 &&
                abs(current_rabbit->location_y - our_y) <= 10 &&
                current_rabbit->is_adult &&
                current_rabbit->gender == MALE &&
                current_rabbit != rabbit) {

                rabbit->is_pregnant = true;
                rabbit->pregnancy_remaining = Nk;
                return;
            }
            current_rabbit = current_rabbit->next;
        }
    }
}

void remove_rabbit(Rabbit* rabbit) {
    // since ->next and ->prev are void* they need to be casted to Rabbit*
    if (rabbit->prev != NULL) {
        ((Rabbit*) rabbit->prev)->next = rabbit->next;
    }
    if (rabbit->next != NULL) {
        ((Rabbit*) rabbit->next)->prev = rabbit->prev;
    }

    free(rabbit);
}

void print_rabbit(Rabbit* rabbit) {
    printf("Rabbit: x=%d, y=%d, age=%d, gender=%s, is_pregnant=%s, pregnancy_remaining=%d, is_adult=%d\n",
        rabbit->location_x,
        rabbit->location_y,
        rabbit->age,
        rabbit->gender == 1 ? "MALE" : "FEMALE",
        rabbit->is_pregnant ? "true" : "false",
        rabbit->pregnancy_remaining,
        rabbit->is_adult
    );
}

void print_all_rabbits(Rabbit* first_rabbit) {
    Rabbit* current_rabbit = first_rabbit;
    while (current_rabbit != NULL) {
        print_rabbit(current_rabbit);
        current_rabbit = current_rabbit->next;
    }
}