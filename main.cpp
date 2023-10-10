#include "npuzzle.hpp"
#include "State.hpp"
#include "Config.hpp"
#include <memory>
#define SIZE 4


int State::n = 0;
int State::size = State::getSideSize() * State::getSideSize();
size_t State::total_states = 0;
std::vector<int> State::target_position;
std::vector<std::vector<uint64_t>> State::hash_grid;
std::unordered_map<uint64_t, std::pair<int, int>> State::transposition_table;
std::default_random_engine rng_engine(1);

using namespace std::chrono;

int main(int argc, char **argv)
{
	srand(time(0));

	State * init_state;
	Config config;

	if (argc == 1)
	{
		grid_format start_grid = generate_grid(SIZE);
		// grid_format start_grid = {0, 8, 3, 6, 7, 5, 4, 1, 2};

/*		while (!is_solvable(start_grid))
		{
			std::cout << "This grid is not solvable, generating a new one." << std::endl << std::endl;
			start_grid = generate_grid(SIZE);	
		}*/
		std::cerr << "before problems" << std::endl;
		init_state = new State(start_grid);
	}
	else
	{
		if(!config.loadNewFile(argv[1]))
		{
			config.printError();
			exit(EXIT_FAILURE);
		}
/*		if (!is_solvable(config.getGrid()))
		{
			std::cerr << "This grid is invalid or is unsolvable\n";
			exit(EXIT_FAILURE);
		}*/
		init_state = new State(config.getGrid());
	}
	if(!init_state)
	{
		std::cerr << "Fatal error\n";
		exit(EXIT_FAILURE);
	}

	std::cout << "---Initial state---" << std::endl;
		
	init_state->display_grid();
	std::cout << std::endl;
	
	auto start = high_resolution_clock::now();
	// if (search_algorithm(init_state) == -1)
		// std::cout << "Solution not found" << std::endl;
	
	SearchResult result = search_algorithm(init_state);
	// SearchResult result = a_star(init_state);
	if (result.success == false)
		std::cout << "Solution not found" << std::endl;

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	for(auto & move : result.path)
	{
		move.display_grid();
		move.display_dir();
		std::cout << std::endl;
	}
	std::cout << "Success : " << result.success << std::endl;
	std::cout << "Steps required to solve :      " << result.path.size() - 1 << std::endl;
	std::cout << "Total number of opened states: " << result.open_states << std::endl;
	std::cout << "Total number of closed states: " << result.closed_states << std::endl;
	std::cout << "total number of states:        " << result.max_states_in_memory << std::endl;
	std::cout << "total number of iterations:    " << result.iterations << std::endl;
	std::cout << "max depth search:              " << result.max_depth << std::endl;
	std::cout << "Transposition table size:      " << result.max_transpositions << std::endl;
	std::cout << "iterations per ms:             " << result.iterations / (duration.count() > 0 ? duration.count() : 1 ) << "/ms" << std::endl;

	std::cout << std::endl << duration.count() << " ms" << std::endl;
	delete init_state;
	return EXIT_SUCCESS;
}
