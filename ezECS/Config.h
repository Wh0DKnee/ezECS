#pragma once


static const int INITIAL_POOL_SIZE = 100;
// Determines for how many new entities we want to make space at once when we exhaust the current pool.
static const int POOL_INCREMENT_SIZE = 10;
static const int MAX_COMPONENTS = 64;
