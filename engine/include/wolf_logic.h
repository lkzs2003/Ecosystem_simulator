#pragma once

#include "wolf.h"
#include "rabbit.h"

Wolf* generate_random_wolf(Wolf* prev);

Wolf* generate_n_random_wolves(int n);

bool wolf_tick(Wolf* wolf, int day, Rabbit* first_rabbit);
void wolf_breeding_tick(Wolf* wolf, Wolf* first_wolf);

void remove_wolf(Wolf* wolf);

void print_wolf(Wolf* wolf);
void print_all_wolves(Wolf* first_wolf);