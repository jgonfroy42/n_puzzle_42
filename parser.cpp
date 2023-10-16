#include "npuzzle.hpp"
#include "Config.hpp"
#include "State.hpp"

State * parser(int argc, char **argv, Config & config)
{
	State * init_state;
	int grid_size = SIZE;
	std::vector<std::string> args;
	args.reserve(argc);

	while(*argv)
		args.push_back(*argv++);

	args.erase(args.begin()); //removing program name from args

	if (!config.setAlgo(args.front()))
	{
		std::cout << "Invalid algo choices are: A_STAR | IDA_STAR" << std::endl;
		return(NULL);
	}
	args.erase(args.begin()); //removing algo name


	if(!config.setHeuristic(args.front()))
	{
		std::cout << "Invalid params, Heuristics choices are: MH | MH+LC | MP" << std::endl;
		return(NULL);
	}
	args.erase(args.begin()); //removing heuristics name

	if (!config.setSearchParams(args.front()))
	{
		std::cout << "Invalid params, SearchParams choies are: UNIFORM | GREEDY" << std::endl;
		return(NULL);
	}
	args.erase(args.begin()); //removing searc param


	//PARSING FILE OR GRID SIZE

	//trying for size number or filename
	if(args.size() != 0 && isOnlyDigits(args.front()))
	{
		grid_size = std::stoi(args.front());
		args.erase(args.begin());		
	}

	//if there is a filename left
	if (args.size() == 0)
	{
		grid_format start_grid = generate_grid(grid_size);

		init_state = new State(start_grid);
		if (!init_state)
		{
			std::cerr << "Fatal Error\n";
			return NULL;
		}
	}
	else
	{
		if(!config.loadNewFile(args.front()))
		{
			config.printError();
			return NULL;
		}

		init_state = new State(config.getGrid());
		if (!init_state)
		{
			std::cerr << "Fatal Error\n";
			return NULL;
		}
	}

	return init_state;
}
