#include "npuzzle.hpp"
#include "Config.hpp"
#include "State.hpp"

int State::n = 0;
int State::size = 0;


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "run ./program <filename>\n";
		exit(EXIT_FAILURE);
	}
	Config config;

	if (!config.loadNewFile(argv[1]))
	{
		config.printError();
		exit(EXIT_FAILURE);
	}

	State state(config.getGridSize(), config.getGrid());

	state.display_grid();
	exit(EXIT_SUCCESS);
}
