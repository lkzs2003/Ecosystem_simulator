#pragma once

#include "rabbit.h"

Rabbit* generate_random_rabbit(Rabbit* prev);
Rabbit* generate_n_random_rabbits(int n);

bool rabbit_tick(Rabbit* rabbit, int day);
void rabbit_breeding_tick(Rabbit* rabbit, Rabbit* first_rabbit);

void remove_rabbit(Rabbit* rabbit);

void print_rabbit(Rabbit* rabbit);
void print_all_rabbits(Rabbit* first_rabbit);