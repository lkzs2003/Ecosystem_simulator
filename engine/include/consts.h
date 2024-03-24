#pragma once



// Base chance of rabbit escaping from a wolf (0-1)
#define RABBIT_ESCAPE_CHANCE (float)0.2

// Per-week escape chance increase
#define RABBIT_ESCAPE_CHANGE_INCREASE (float)0.2

// Island side length (cells)
#define CELLS_WIDTH (int)1000

// Wolf daily travel distance (cells)
#define WOLF_TRAVEL_DISTANCE (int)100

// Wolf smell range
#define WOLF_SMELL_RANGE (int)1

// Wolf pregnancy duration (weeks; father has to catch at least 2 rabbits per week)
#define WOLF_PREGNANCY_DURATION (int)4

// Wolf child protection period (weeks; parents have to catch an extra rabbit per week for all children)
#define WOLF_CHILD_PROTECTION_PERIOD (int)24

// Wolf maximum starvation duration (weeks)
#define WOLF_STARVATION_DURATION (int)4

// Young wolf maximum starvation duration (weeks)
#define YOUNG_WOLF_STARVATION_DURATION (int)3

// Wolf gay percentage (0-100)
#define WOLF_GAY_PERCENTAGE 5