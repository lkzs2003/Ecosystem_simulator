#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "island.h"

Island* engine_init(int seed);

void engine_tick(Island* island);

void engine_deinit(Island* island);