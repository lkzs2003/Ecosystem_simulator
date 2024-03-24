#pragma once

// Initializes the random number generator with the given seed (srand)
void rng_init(int seed);

// Returns a random number between min and max (inclusive)
int rng_rand(int min, int max);