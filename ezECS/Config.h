#pragma once


static const int NUM_INITIAL_FREE_ID = 100;
// Determines for how many new entities we want to make space at once when we exhaust the current pool.
static const int NUM_FREE_ID_INCREMENT = 10;
static const int MAX_COMPONENTS = 64;
