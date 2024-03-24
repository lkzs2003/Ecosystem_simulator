#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "rng.h"

#include "engine.h"
#include "rabbit_logic.h"


int main()
{
    int seed = 123123123;
    printf("Random seed: %d\n", seed);

    Island* island = engine_init(seed);

    for (int i = 0; i < 100; i++) {

        printf("Day %d,", island->day);
        engine_tick(island);

        // Print population: male adult, female adult, male child, female child
        int ma = 0;
        int fa = 0;
        int mc = 0;
        int fc = 0;
        Rabbit* current_rabbit = island->first_rabbit;
        while (current_rabbit != NULL) {
            if (current_rabbit->is_adult) {
                if (current_rabbit->gender == MALE) {
                    ma++;
                } else {
                    fa++;
                }
            } else {
                if (current_rabbit->gender == MALE) {
                    mc++;
                } else {
                    fc++;
                }
            }
            current_rabbit = current_rabbit->next;
        }
        printf("%d,%d,%d,%d,", ma, fa, mc, fc);

        int aw = 0;
        int cw = 0;
        Wolf* current_wolf = island->first_wolf;
        while (current_wolf != NULL) {
            if (current_wolf->is_adult) {
                aw++;
            } else {
                cw++;
            }
            current_wolf = current_wolf->next;
        }
        printf("%d,%d\n", aw, cw);

    }

    engine_deinit(island);

    return 0;
}
