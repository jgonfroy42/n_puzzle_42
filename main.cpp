#include "main.hpp"
#include "State.hpp"

int n = 8;
using namespace std::chrono;


std::vector<State> search_algorithm(State *init_state);

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
	if (n % 2 != 0)
	{
		if (inversion % 2 == 0)
			return true;
		return false;
	}
	
	//if n is even and inversion odd, solvable if:
	//- blank in even row (from bottom) and inversion odd
	//- or, blank in odd row and inversion even

	int row_from_bottom = n - blank_index / n;
	if (row_from_bottom % 2 == 0 && inversion % 2 != 0)
		return true;
	if (row_from_bottom % 2 != 0 && inversion % 2 == 0)
		return true;

	return false;
}

int main()
{
	srand(time(0));
	State *init_state = new State(generate_grid(n));
	

	while (!is_solvable(init_state->grid))
	{
		std::cout << "This grid is not solvable, generating a new one." << std::endl << std::endl;
		init_state->grid = generate_grid(n);	
	}

	std::cout << "---Initial state---" << std::endl;
		
	init_state->display_grid();

	auto start = high_resolution_clock::now();
	std::vector<State> path = search_algorithm(init_state);
	auto stop = high_resolution_clock::now();

	std::cout << std::endl << "---Solution---" << std::endl;

	for (auto & it : path)
	{
		it.display_grid();
		std::cout << std::endl;
	}

	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << std::endl << duration.count() << " ms" << std::endl;
	delete init_state;
}

//okay so "old" algorithm had a loop where
/*

	while todo ! empty
		take the first one
		insert it to visited
		expand children
		for each children
			if winning state finish
			else if !already visited push it to todo


	new proposition is

	while todo ! empty
*/

std::vector<State> create_path(const State *ending_state)
{
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

std::vector<State> search_algorithm(State *init_state) //now search algorithm has ownership of every state storage and only returns a vector containing a valid path
{

	grid_format winning_state = get_winning_grid(init_state->n);
	// std::set<std::vector<int>> visited; old visited set
	auto cmp_state_grid = [](const State & lhs, const State & rhs){return lhs.grid < rhs.grid;};
	std::set<State, decltype(cmp_state_grid)> storage; //new visited set behave very similarly only it doesnt store pointers
	//and it is now called "storage"
	//we let set handle the allocation and deallocations
	
	//toDo still stores pointers since we only want to point to different states that we have previously generated and stored
	auto cmp = [](const State *lhs, const State *rhs) { return lhs->score > rhs->score; };
	std::priority_queue<const State*, std::vector<const State*>, decltype(cmp)> toDo(cmp);
	
	toDo.push(init_state);
	storage.insert(*init_state);

	while (!toDo.empty())
	{
		const State *current = toDo.top();
		toDo.pop();

		for (auto & move : current->get_possible_moves()) //very important here to add the & after auto to avoid useless copies
		{
			auto ret = storage.insert(move); //inserting it in storage
			if (ret.second == false) //if insertion failed becasue there was already a similar state then this value will be false
				continue;

			if (move.grid == winning_state)
				return create_path(&(*ret.first));
			toDo.push(&(*ret.first));
		}
	}

	return create_path(init_state);
}
