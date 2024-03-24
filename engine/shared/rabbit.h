#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "consts.h"
#include "animal.h"

typedef struct Rabbit {
    uint16_t location_x;
    uint16_t location_y;

    uint16_t age; // weeks
    Gender gender;

    bool is_pregnant;
    uint16_t pregnancy_remaining; // weeks
    uint16_t fed_weeks; // weeks

    bool is_adult;

    void* prev;
    void* next;
} Rabbit;