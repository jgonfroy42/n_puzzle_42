#include <time.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>

#include "State.hpp"


#include <chrono>
using namespace std::chrono;

int n = 5;

State	*search_algorithm(State *init_state);

std::vector<int>	get_winning_grid(int size)
{
	std::vector<int> grid;

	for (int i = 0; i < size*size; i++)
		grid.push_back(i + 1);
	grid[grid.size() - 1] = 0;

	return grid;
}

/*
 * pourquoi pas utiliser cette seulemement dans le constructeur vide de State ?
 * */

std::vector<int> generate_grid(int size)
{
	//std::shuffle(container.begin(), container.end(), std::random_device());
	std::vector<int> tiles;
	std::vector<int> grid;
	
	for (int i = 0; i < size*size; i++)
		tiles.push_back(i);
	
	while (!tiles.empty())
	{
		int random = rand() % tiles.size();
		grid.push_back(tiles[random]);
		tiles.erase(tiles.begin() + random);
	}
	return grid;
}

const bool is_solvable(const std::vector<int> &grid)
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
	
	auto start = high_resolution_clock::now();

	while (!is_solvable(init_state->grid))
	{
		std::cout << "This grid is not solvable, generating a new one." << std::endl << std::endl;
		init_state->grid = generate_grid(n);	
	}

	std::cout << "---Initial state---" << std::endl;
		
	init_state->display_grid();

	State *solution = search_algorithm(init_state);
	std::stack<State*> path;	

	std::cout << std::endl << "---Solution---" << std::endl;

	while (solution->parent != NULL)
	{
		path.push(solution);
		solution = solution->parent;	
	}
	path.push(solution);

	while (!path.empty())
	{
		State *tmp = path.top();
		path.pop();
		tmp->display_grid();
		std::cout << std::endl;
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << std::endl << duration.count() << " ms" << std::endl;
}

State	*search_algorithm(State *init_state)
{
	
	std::vector<int> winning_state = get_winning_grid(init_state->n);
	std::vector<std::vector<int>> visited;
	
	auto cmp = [](State *lhs, State *rhs) { return lhs->score > rhs->score; };
	std::priority_queue<State*, std::vector<State*>, decltype(cmp)> toDo(cmp);
	
	toDo.push(init_state);

	while (!toDo.empty())
	{
		State *current = toDo.top();
		toDo.pop();
		visited.push_back(current->get_grid());
		

		for (auto move : current->get_possible_moves())
		{
			if (*move == winning_state)
				return move;
			if (find(visited.begin(), visited.end(), move->get_grid()) == visited.end())
			{
				toDo.push(move);
				continue ;
			}
			delete move;
		}
	}

	return init_state;
}
