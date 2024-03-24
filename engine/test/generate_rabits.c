#include "consts.h"
#include "params.h"

#include "island.h"
#include "animal.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "rng.h"

Rabbit* generateRabbit(Rabbit* prev);

int main(void) {
    printf("Hello, world! %f\n", RABBIT_ESCAPE_CHANCE);

    rng_init(123123);

    Island island;

    Rabbit* rabbit = generateRabbit(NULL);

    island.first_rabbit = rabbit;
    
    for (int i = 0; i <= 100; i++) {
        rabbit->next = generateRabbit(rabbit);
        rabbit = rabbit->next;
    }

    Rabbit* current_rabbit = island.first_rabbit;
    while (current_rabbit != NULL) {
        printf("Rabbit: %d, %d\n", current_rabbit->location_x, current_rabbit->location_y);
        printf("prev: %p, next: %p\n", current_rabbit->prev, current_rabbit->next);
        current_rabbit = current_rabbit->next;
    }

    return -0;
}

Rabbit* generateRabbit(Rabbit* prev) {
    Rabbit* rabbit = malloc(sizeof(Rabbit));

    rabbit->location_x = rng_rand(0, 999);
    rabbit->location_y = rng_rand(0, 999);
    rabbit->age = 0;
    rabbit->gender = CHAOS;
    rabbit->is_pregnant = false;
    rabbit->pregnancy_remaining = 0;
    rabbit->is_adult = false;

    rabbit->prev = prev;

    return rabbit;
}