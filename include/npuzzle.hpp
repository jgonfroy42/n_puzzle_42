#pragma once

#include <time.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include <limits.h>
#include <chrono>
#include <algorithm>
#include <random>



typedef uint_least8_t cell_size;
typedef std::vector<int> grid_format;
typedef cell_size * optimized_grid;

extern std::default_random_engine rng_engine;

#define MAX_SIZE 15
