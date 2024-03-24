#include <stdio.h>
#include <stdlib.h>

void rng_init(int seed) {
    srand(seed);
}

int rng_rand(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}
