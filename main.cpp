#include "npuzzle.hpp"
#include "State.hpp"
#include "Config.hpp"
#define SIZE 6


int State::n = 0;
int State::size = State::getSideSize() * State::getSideSize();
std::default_random_engine rng_engine(1);

using namespace std::chrono;


int	search_algorithm(State *init_state);
int a_star(State *init_state);
int deepening_search(State &state, int g, int palier, State &winning_state, std::vector<State> & end_path);

grid_format	get_winning_grid(int size)
{
	grid_format grid;

	for (cell_size i = 0; i < size*size; i++)
		grid.push_back(i + 1);
	grid[grid.size() - 1] = 0;

	return grid;
}

/*
 * pourquoi pas utiliser cette seulemement dans le constructeur vide de State ?
 * */

grid_format generate_grid(int size)
{
	//std::shuffle(container.begin(), container.end(), std::random_device());
	grid_format tiles;
	grid_format grid;
	
	for (cell_size i = 0; (int)i < size*size; i++)
		tiles.push_back(i);
	
	for (auto & it : tiles)
		std::cerr << int(it) << std::endl;
	
	while (!tiles.empty())
	{
		int random = rand() % tiles.size();
		grid.push_back(tiles[random]);
		tiles.erase(tiles.begin() + random);
	}
	return grid;
}

grid_format generate_custom_grid()
{

	grid_format ret = {0, 8, 3, 6, 7, 5, 4, 1, 2};
	return ret;
}

bool is_solvable(const grid_format &grid)
{
	int inversion = 0; 
	int blank_index = grid.size() - 1; //seule position où l'index ne sera pas mis à jour dans la boucle

	int nsize = sqrt(grid.size());
	//testing that the grid has valid tiles
	grid_format copy = grid;
	std::sort(copy.begin(), copy.end());

	for(int i = 0;i < (int)copy.size(); i++)
	{
		if (copy[i] != i)
			return false;
	}


	for (size_t i = 0; i < grid.size() - 1; i++)
	{
		if (grid[i] == 0)
		{
			blank_index = i;
			continue;
 		}
		for (size_t j = i + 1; j < grid.size(); j++)
		{
			if (grid[j] != 0 && grid[j] < grid[i])
				inversion++;
		}
	}
 
	//if n is odd, solvable if number of inversion in even
	if (nsize % 2 != 0)
	{
		if (inversion % 2 == 0)
			return true;
		return false;
	}
	
	//if n is even and inversion odd, solvable if:
	//- blank in even row (from bottom) and inversion odd
	//- or, blank in odd row and inversion even

	int row_from_bottom = nsize - blank_index / nsize;
	if (row_from_bottom % 2 == 0 && inversion % 2 != 0)
		return true;
	if (row_from_bottom % 2 != 0 && inversion % 2 == 0)
		return true;

	return false;
}

int main(int argc, char **argv)
{
	srand(time(0));

	State * init_state;
	Config config;

	if (argc == 1)
	{
		grid_format start_grid = generate_grid(SIZE);
		// grid_format start_grid = {0, 8, 3, 6, 7, 5, 4, 1, 2};

		while (!is_solvable(start_grid))
		{
			std::cout << "This grid is not solvable, generating a new one." << std::endl << std::endl;
			start_grid = generate_grid(SIZE);	
		}
		init_state = new State(start_grid);
	}
	else
	{
		if(!config.loadNewFile(argv[1]))
		{
			config.printError();
			exit(EXIT_FAILURE);
		}
		if (!is_solvable(config.getGrid()))
		{
			std::cerr << "This grid is invalid or is unsolvable\n";
			exit(EXIT_FAILURE);
		}
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
	
	if (a_star(init_state) == -1)
		std::cout << "Solution not found" << std::endl;

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << std::endl << duration.count() << " ms" << std::endl;

	delete init_state;
}

std::vector<State> create_path(const State *ending_state)
{
	std::cout << "creating path\n";
	std::stack<const State *> temp;
	std::vector<State> ret;

	while (ending_state != NULL)
	{
		temp.push(ending_state);
		ending_state = ending_state->parent;
	
	}

	while (!temp.empty())
	{
		ret.push_back(*temp.top());
		temp.pop();
	}

	return ret;
}

int	search_algorithm(State *init_state)
{
	int palier = init_state->score;
	State winning_state(get_winning_grid(State::getSideSize()));
	std::vector<State> end_path;

//créer le chemin dans la fonction init sans avoir besoin de retour ?	
	while (true)
	{
		auto ret = deepening_search(*init_state, 0, palier, winning_state, end_path);
		if (ret == 0)
		{
			std::cout << "---Solution---" << std::endl;
			for (auto step : end_path)
			{
				step.display_grid();
				std::cout << std::endl;
			}
			std::cout << "Solution found in " << end_path.size() << " steps" << std::endl;
			return 0;
		}
		palier = ret;
		std::cout << "increasing threshold to :" << palier << std::endl;
	}

	return -1;
}

int deepening_search(State &state, int g, int palier, State &winning_state, std::vector<State> & end_path)
{

	int f = g + state.score;
	if (f > palier)
		return f;
	if (state == winning_state)
		return 0;

	int min = INT_MAX;
	for (auto &move : state.get_possible_moves())
	{
		auto ret = deepening_search(move, g + 1, palier, winning_state, end_path);
		if (ret == 0)
		{
			if (end_path.empty())
				end_path = create_path(&move);
			return 0;
		}
		if (ret < min)
			min = ret;
	} 
	return min;
}

int a_star(State *init_state)
{
	State winning_state(get_winning_grid(init_state->getSideSize()));

	// auto cmp_ptr = [](State *lhs, State *rhs) {return *lhs < *rhs;};
	auto cmp = [](State *lhs, State *rhs) { return lhs->score > rhs->score;};

	// std::set<State*, decltype(cmp_ptr)> visited(cmp_ptr);

	std::unordered_map<std::string, State*> visited_bis;
	std::string key;

	std::priority_queue<State*, std::vector<State*>, decltype(cmp)> toDo(cmp);

	toDo.push(init_state);

	while(!toDo.empty())
	{
		State *current = toDo.top();
		toDo.pop();
		// visited.insert(current);
		key.assign(reinterpret_cast<char *>(current->get_grid()), current->getTotalSize() * sizeof(cell_size) / sizeof(int));
		std::cerr << key << std::endl;
		visited_bis.insert(std::make_pair(key, current));

		for (auto & move : current->get_possible_moves())
		{
			key.assign(reinterpret_cast<char *>(move.get_grid()), move.getTotalSize() * sizeof(cell_size) / sizeof(int));
			if (move == winning_state)
			{
				std::cout << "---Solution---" << std::endl;
				auto path = create_path(&move);
				for (auto step : path)
				{
					step.display_grid();
					std::cout << std::endl;
				}
				std::cout << "Solution found in " << path.size() << " steps" << std::endl;
				return 0;
			}

			if(visited_bis.find(key) == visited_bis.end())
			{
				State * new_move = new State(move);
				toDo.push(new_move);
				visited_bis.insert(std::make_pair(key, new_move));
				continue;
			}
		}
	}
	return -1;
}
