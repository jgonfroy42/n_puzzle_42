#include "npuzzle.hpp"
#include "State.hpp"
#include "Config.hpp"
#include <memory>


std::default_random_engine rng_engine(1);

using namespace std::chrono;

int main(int argc, char **argv)
{
	srand(time(0));

	State * init_state;
	Config config;
	std::vector<std::string> args;

	init_state = parser(argc, argv, config);
	if(!init_state)
		exit(EXIT_FAILURE);
	State::set_config(config);

	std::cout << "---Initial state---" << std::endl;
		
	init_state->display_grid();
	std::cout << std::endl;
	
	auto start = high_resolution_clock::now();
	// if (search_algorithm(init_state) == -1)
		// std::cout << "Solution not found" << std::endl;
	
	SearchResult result;
	if (config.getAlgo() == IDA_STAR)
		result = search_algorithm(init_state);
	else
		result = a_star(init_state);
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
