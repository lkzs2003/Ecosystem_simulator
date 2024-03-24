#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "consts.h"
#include "animal.h"

typedef struct Wolf Wolf;

typedef struct Wolf {
    Wolf* prev;
    Wolf* next;

    uint16_t location_x;
    uint16_t location_y;

    uint16_t age; // weeks
    Gender gender;

    bool is_pregnant;
    uint16_t pregnancy_remaining; // weeks
    uint16_t starving_time; // weeks
    uint16_t offspring_count;
    bool has_eaten;

    bool is_adult;

    bool is_gay;

    bool died;

    Wolf* partner;
    Wolf* first_child; // Only for mother
} Wolf;