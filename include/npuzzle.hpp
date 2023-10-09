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
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <exception>
#include <iomanip>


typedef uint_least8_t cell_size;
typedef std::vector<int> grid_format;
typedef cell_size * optimized_grid;

class State;
struct SearchResult
{
	bool success = false;
	int open_states = 0;
	int closed_states = 0;
	int max_states_in_memory = 0;
	int iterations = 0;
	std::vector<State> path;
};

extern std::default_random_engine rng_engine;
SearchResult	a_star(State *init_state);
SearchResult	search_algorithm(State *init_state);
int deepening_search(int palier, int g, State &winning_state, std::vector<State> & end_path, std::unordered_set<uint64_t> & visited);
grid_format	get_winning_grid(int size);
grid_format generate_grid(int size);
grid_format generate_custom_grid();
bool is_solvable(const grid_format &grid);

#define MAX_SIZE 15
