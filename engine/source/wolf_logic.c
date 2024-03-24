#include "wolf.h"
#include "rabbit.h"
#include <stdio.h>
#include <stdlib.h>
#include "rng.h"
#include "params.h"
#include "rabbit_logic.h"
#include "wolf_logic.h"
#include "utils.h"

//#define DEBUG

#ifdef DEBUG
// For debugging purposes
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

Wolf* generate_random_wolf(Wolf* prev){
    Wolf* wolf = malloc(sizeof(Wolf));
    wolf->location_x = rng_rand(1, L);
    wolf->location_y = rng_rand(1, L);
    wolf->gender = rng_rand(0, 1) == 1 ? MALE : FEMALE;
    wolf->is_pregnant = false;
    wolf->pregnancy_remaining = 0;
    wolf->is_adult = true;
    wolf->offspring_count = 0;
    wolf->has_eaten = false;
    wolf->starving_time = 0;
    wolf->age = rng_rand(M, S);
    wolf->prev = prev;
    wolf->partner = NULL;
    wolf->first_child = NULL;

    wolf->died = false;
    wolf->is_gay = rng_rand(0, 100) <= WOLF_GAY_PERCENTAGE;

    if (wolf->is_gay) {
        DEBUG_PRINT("WOLF IS WOKE!! :O\n");
    }

    return wolf;
}

Wolf* generate_random_child_wolf(Wolf* prev, Wolf* parent) {
    Wolf* wolf = malloc(sizeof(Wolf));
    wolf->location_x = parent->location_x;
    wolf->location_y = parent->location_y;
    wolf->gender = rng_rand(0, 1) == 1 ? MALE : FEMALE;
    wolf->is_pregnant = false;
    wolf->pregnancy_remaining = 0;
    wolf->is_adult = false;
    wolf->offspring_count = 0;
    wolf->has_eaten = false;
    wolf->starving_time = 0;
    wolf->age = 0;
    wolf->prev = prev;
    wolf->partner = NULL;
    wolf->first_child = NULL;

    wolf->died = false;
    wolf->is_gay = rng_rand(0, 100) <= WOLF_GAY_PERCENTAGE;

    if (wolf->is_gay) {
        DEBUG_PRINT("WOLF IS WOKE!! :O\n");
    }

    return wolf;
}

Wolf* generate_n_random_wolves(int n) {
    Wolf* current_wolf = NULL;
    Wolf* first_wolf = NULL;
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            first_wolf = generate_random_wolf(NULL);
            current_wolf = first_wolf;
        } else {
            current_wolf->next = generate_random_wolf(current_wolf);
            current_wolf = current_wolf->next;
        }
    }
    return first_wolf;
}

// Returns true if the wolf is still alive, false otherwise
bool wolf_tick(Wolf* wolf, int day, Rabbit* first_rabbit) {
    Wolf* partner = wolf->partner;

    // A wolf stops moving when ->has_eaten is true
    // This resets to false every week
    if (wolf->has_eaten) {
        if (day % 7 == 0) {
            wolf->has_eaten = false;
        } else {
            // If the wolf has a pregnant partner, it needs to hunt for them
            if (wolf->partner != NULL && partner->is_pregnant) {
                DEBUG_PRINT("Partner is pregnant, so continuing\n");
            } else {
                return true;
            }
        }
    }

    // ==== MOVEMENT ====
    // A wolf can move a total of (CELLS_WIDTH) cells per day
    // For now, we will move it randomly, 0-50 cells in x direction, 0-50 cells in y direction
    // (TODO: improve and reconsider this)
    int delta_x = rng_rand(-(CELLS_WIDTH/2), (CELLS_WIDTH/2));
    int delta_y = rng_rand(-(CELLS_WIDTH/2), (CELLS_WIDTH/2));

    wolf->location_x += delta_x;
    wolf->location_y += delta_y;

    // If the wolf is outside the island, move it back inside
    wolf->location_x = clamp(1, wolf->location_x, L);
    wolf->location_y = clamp(1, wolf->location_y, L);

    // ==== AGE ====
    // We store age in weeks so increment it by one when day is divisible by 7
    if (day % 7 == 0) {
        wolf->age++;
    }

    if (wolf->age > WOLF_CHILD_PROTECTION_PERIOD) {
        wolf->is_adult = true;
    }

    // Wolfs do not die of old age, they die of starvation

    // ==== STARVATION ====
    // Adults: if they have not eaten for WOLF_STARVATION_DURATION weeks, they die
    // Children: if they have not eaten for YOUNG_WOLF_STARVATION_DURATION weeks, they die
    if (wolf->is_adult) {
        if (wolf->starving_time >= WOLF_STARVATION_DURATION) {
            DEBUG_PRINT("Wolf died of starvation\n");
            wolf->died = true;
            return false;
        }
    } else {
        if (wolf->starving_time >= YOUNG_WOLF_STARVATION_DURATION) {
            wolf->died = true;
            return false;
        }
    }

    // ==== FEEDING ====
    // If a wolf is on the same cell as a rabbit, it eats it
    // This only happens in RABBIT_ESCAPE_CHANCE % of the cases
    // However, every week the wolf does not eat, the chance increases by RABBIT_ESCAPE_CHANGE_INCREASE
    // Iterate through all rabbits and check if any of them is on the same cell
    // Pregnant wolves do go hunting (they rely on their partner to bring them food)

    // TODO: feeding in the protection period

    Wolf* hunting_target;
    if (wolf->has_eaten && wolf->partner != NULL && partner->is_pregnant) {
        DEBUG_PRINT("Partner is pregnant, so hunting for partner\n");
        hunting_target = wolf->partner;
    } else {
        DEBUG_PRINT("Partner is not pregnant, so hunting for itself\n");
        hunting_target = wolf;
    }

    if (wolf->is_adult && !wolf->is_pregnant) {
        Rabbit* current_rabbit = first_rabbit;
        while (current_rabbit != NULL) {
            int rabbit_escape_chance = (RABBIT_ESCAPE_CHANCE + wolf->starving_time * RABBIT_ESCAPE_CHANGE_INCREASE) * 100;

            if (rng_rand(0, 100) < (rabbit_escape_chance)) {
                // The rabbit escaped, DO NOT continue to the next rabbit
                current_rabbit = current_rabbit->next;
                DEBUG_PRINT("Rabbit escaped @ chance %d \n", rabbit_escape_chance);
                break;
            } else {
                DEBUG_PRINT("Rabbit did not escape @ chance %d \n", rabbit_escape_chance);
            }

            if (current_rabbit->location_x == wolf->location_x && current_rabbit->location_y == wolf->location_y) {
                // The wolf has eaten the rabbit
                // Reset the starving time
                hunting_target->starving_time = 0;
                // Remove the rabbit
                remove_rabbit(current_rabbit);
                hunting_target->has_eaten = true;

                break;
            }
            current_rabbit = current_rabbit->next;
        }
    }

    if (!wolf->has_eaten && day % 7 == 0) {
        wolf->starving_time++;
    }

    // ==== PREGNANCY ====
    // If the wolf is pregnant, decrement the remaining time (every 7 days)
    if (wolf->is_pregnant) {
        if (day % 7 == 0) {
            wolf->pregnancy_remaining--;
            DEBUG_PRINT("[PREGNANCY] Pregnancy remaining: %d\n", wolf->pregnancy_remaining);
        }

        // If the pregnancy is over, give birth to (MINw, MAXw) wolves (randomly)
        if (wolf->pregnancy_remaining == 0) {
            int num_offspring = rng_rand(MINw, MAXw);

            // Generate and add the offspring
            for (int i = 0; i < num_offspring; i++) {
                DEBUG_PRINT("[PREGNANCY] Generating offspring\n");
                Wolf* offspring = generate_random_child_wolf(wolf, wolf);

                // Add the offspring to the list (NOT the main list, but the children list of the parent)
                if (wolf->first_child == NULL) {
                    // If the wolf is the first one, set it as the first one
                    wolf->first_child = offspring;
                } else {
                    // If the wolf is not the first one, insert it between the previous and the current wolf
                    Wolf* prev_wolf = wolf->first_child;
                    while (prev_wolf->next != NULL) {
                        prev_wolf = prev_wolf->next;
                    }
                    prev_wolf->next = offspring;
                    offspring->prev = prev_wolf;
                }

                // Clear pregnancy flags
                wolf->is_pregnant = false;
                wolf->pregnancy_remaining = 0;
            }
        }
    }

    // ==== CHILDREN ====
    // Every week, the wolf's children grow up one week
    // Once they hit WOLF_CHILD_PROTECTION_PERIOD, they become adults and are added to the main list
    // (inserted between the parent and the next wolf)
    Wolf* current_child = wolf->first_child;
    while (current_child != NULL) {
        if (day % 7 == 0) {
            current_child->age++;
        }

        if (current_child->age > WOLF_CHILD_PROTECTION_PERIOD) {
            DEBUG_PRINT("[PREGNANCY] Child became adult\n");
            current_child->is_adult = true;
            // Insert the child between the parent and the next wolf
            Wolf* next_wolf = wolf->next;
            wolf->next = current_child;
            current_child->prev = wolf;
            current_child->next = next_wolf;
            if (next_wolf != NULL) {
                next_wolf->prev = current_child;
            }
            current_child = current_child->next;
        } else {
            current_child = current_child->next;
        }
    }

    // On Linux, while running under python, the wolf->next pointer can sometimes get corrupted
    // We are unsure why, sounds like a memory allocator edge case, so just use this hack to fix it for now
    if (wolf->next == (Wolf*)0xffffff00ffffff) {
        printf("wolf->next corrupted\n");
        wolf->next = NULL;
    }

    return true;
}

void wolf_breeding_tick(Wolf* wolf, Wolf* first_wolf) {
    // ==== BREEDING ====
    // If the wolf is an adult FEMALE, not pregnant, not with a partner, not with current children,
    // and within 20 (TODO: TBD) cells of a MALE wolf that's not with a partner, 
    // not with current children and adult, it becomes pregnant

    if (wolf->gender == FEMALE && 
        !wolf->is_pregnant && 
        wolf->is_adult && 
        wolf->first_child == NULL &&
        wolf->partner == NULL) {

        if (wolf->is_gay) {
            DEBUG_PRINT("WOLF IS A TRANSBIAN!! :O\n");
            return;
        }

        // Iterate through all wolves and check if any of them is within 20 cells and meets the criteria
        int our_x = wolf->location_x;
        int our_y = wolf->location_y;

        Wolf* current_wolf = first_wolf;
        while (current_wolf != NULL) {
            if (abs(current_wolf->location_x - our_x) <= 10 &&
                abs(current_wolf->location_y - our_y) <= 10 &&
                current_wolf->is_adult &&
                current_wolf->gender == MALE &&
                current_wolf != wolf &&
                current_wolf->partner == NULL) {

                if (current_wolf->is_gay) {
                    DEBUG_PRINT("WOLF IS GAY!! :O\n");
                    break;
                }

                wolf->is_pregnant = true;
                wolf->pregnancy_remaining = Nw;
                wolf->partner = current_wolf;
                current_wolf->partner = wolf;
                DEBUG_PRINT("Wolf became pregnant :3\n");
                return;
            }
            current_wolf = current_wolf->next;
        }

        DEBUG_PRINT("Wolf did not become pregnant :/\n");
    }
    
}

void remove_wolf(Wolf* wolf) {
    // since ->next and ->prev are void* they need to be casted to Wolf*
    if (wolf->prev != NULL) {
//        printf("wolf->prev is not NULL\n");
        wolf->prev->next = wolf->next;
    }
    if (wolf->next != NULL) {
//        printf("wolf->next is not NULL\n");
        wolf->next->prev = wolf->prev;
    }

    // Clear the partner's partner pointer
    if (wolf->partner != NULL) {
//        printf("wolf->partner is not NULL\n");
        wolf->partner->partner = NULL;
    }

    //DEBUG_PRINT("free() called on wolf @ %d %d\n", wolf->location_x, wolf->location_y);
    //free(wolf); // TODO: eventually re-enable this (however it seems like this does not cause any memory leaks)
}

void print_wolf(Wolf* wolf) {
    printf("Wolf: x=%d, y=%d, age=%d, gender=%s, is_pregnant=%s, pregnancy_remaining=%d, is_adult=%d, starving_time=%d\n",
        wolf->location_x,
        wolf->location_y,
        wolf->age,
        wolf->gender == 1 ? "MALE" : "FEMALE",
        wolf->is_pregnant ? "true" : "false",
        wolf->pregnancy_remaining,
        wolf->is_adult,
        wolf->starving_time
    );
}

void print_all_wolves(Wolf* first_wolf) {
    Wolf* current_wolf = first_wolf;
    while (current_wolf != NULL) {
        print_wolf(current_wolf);
        current_wolf = current_wolf->next;
    }
}