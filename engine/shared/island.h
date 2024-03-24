#pragma once

#include "rabbit.h"
#include "wolf.h"

typedef struct {
    Rabbit* first_rabbit;
    Wolf* first_wolf;
    int day;
} Island;