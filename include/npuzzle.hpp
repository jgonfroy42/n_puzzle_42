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
#include <bitset>

#define SIZE 4

typedef uint_least8_t cell_size;
typedef std::vector<int> grid_format;
typedef cell_size * optimized_grid;

class State;
class Config;
struct SearchResult
{
	bool success = false;
	int open_states = 0;
	int closed_states = 0;
	int max_states_in_memory = 0;
	int iterations = 0;
	int max_depth = 0;
	int max_transpositions = 0;
	std::vector<State> path;
};

enum e_algo
{
	DEFAULT_ALGO,
	A_STAR,
	IDA_STAR,
};

enum eval
{
	MANHATTAN,
	HYBRID,
	MISSPLACED_TILES,
};

enum Search_params
{
	GREEDY,
	UNIFORM_COST
};

extern std::default_random_engine rng_engine;


State * parser(int argc, char **argv, Config & config);

SearchResult	a_star(State *init_state);
SearchResult	search_algorithm(State *init_state);
int deepening_search(int palier, int g, State &winning_state, std::vector<State> & end_path, SearchResult & search);
grid_format	get_winning_grid(int size);
grid_format generate_grid(int size);
grid_format generate_custom_grid();
bool is_solvable(const grid_format &grid);

uint64_t generate_random_uint64(void);
bool isOnlyDigit_and_space(std::string & line);
bool isOnlyDigits(std::string & line);
void remove_comments(std::string & line);


#define MAX_SIZE 15
