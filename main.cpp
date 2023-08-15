#include "npuzzle.hpp"
#include "State.hpp"

int State::n = 4;
int State::size = 16;
std::default_random_engine rng_engine(1);

using namespace std::chrono;


int	search_algorithm(State *init_state);
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
	
	for (cell_size i = 0; i < size*size; i++)
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

const bool is_solvable(const grid_format &grid)
{
	int inversion = 0; 
	int blank_index = grid.size() - 1; //seule position où l'index ne sera pas mis à jour dans la boucle

	for (int i = 0; i < grid.size() - 1; i++)
	{
		if (grid[i] == 0)
		{
			blank_index = i;
			continue;
 		}
		for (int j = i + 1; j < grid.size(); j++)
		{
			if (grid[j] != 0 && grid[j] < grid[i])
				inversion++;
		}
	}
 
	//if n is odd, solvable if number of inversion in even
	if (State::getSideSize() % 2 != 0)
	{
		if (inversion % 2 == 0)
			return true;
		return false;
	}
	
	//if n is even and inversion odd, solvable if:
	//- blank in even row (from bottom) and inversion odd
	//- or, blank in odd row and inversion even

	int row_from_bottom = State::getSideSize() - blank_index / State::getSideSize();
	if (row_from_bottom % 2 == 0 && inversion % 2 != 0)
		return true;
	if (row_from_bottom % 2 != 0 && inversion % 2 == 0)
		return true;

	return false;
}

int main()
{
	srand(time(0));
	grid_format start_grid = generate_grid(State::getSideSize());

	while (!is_solvable(start_grid))
	{
		std::cout << "This grid is not solvable, generating a new one." << std::endl << std::endl;
		start_grid = generate_grid(State::getSideSize());	
	}

	std::cout << "---Initial state---" << std::endl;
		
	State *init_state = new State(start_grid);
	init_state->display_grid();
	std::cout << std::endl;

	auto start = high_resolution_clock::now();
	if (search_algorithm(init_state) == -1)
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
			std::cout << "Solutiion found in " << end_path.size() << " steps" << std::endl;
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
