#include <time.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>

#include <chrono>
using namespace std::chrono;

#define SIZE 4

std::vector<int>	search_algorithm(std::vector<int> grid);
void	write_path(std::vector<int> grid, std::vector<int> path, int n);
int	calculate_score(std::vector<int> grid);

std::vector<int>	get_winning_grid(int size)
{
	std::vector<int> grid;

	for (int i = 0; i < size*size; i++)
		grid.push_back(i + 1);
	grid[grid.size() - 1] = 0;

	return grid;
}

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
	if (SIZE % 2 != 0)
	{
		if (inversion % 2 == 0)
			return true;
		return false;
	}
	
	//if n is even and inversion odd, solvable if:
	//- blank in even row (from bottom) and inversion odd
	//- or, blank in odd row and inversion even

	int row_from_bottom = SIZE - blank_index / SIZE;
	if (row_from_bottom % 2 == 0 && inversion % 2 != 0)
		return true;
	if (row_from_bottom % 2 != 0 && inversion % 2 == 0)
		return true;

	return false;
}

int main()
{
	srand(time(0));
	std::vector<int> grid = generate_grid(SIZE);
	
	auto start = high_resolution_clock::now();

	while (!is_solvable(grid))
	{
		std::cout << "This grid is not solvable, generating a new one." << std::endl << std::endl;
		grid = generate_grid(SIZE);	
	}

	grid.push_back(calculate_score(grid));
	std::vector<int> path = search_algorithm(grid);	
	
	std::cout << std::endl << "---Solution---" << std::endl;
	write_path(grid, path, SIZE);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << std::endl << duration.count() << " ms" << std::endl;
}

void	write_path(std::vector<int> grid, std::vector<int> path, int n)
{
	int last_index = path[0];
	
	for (int new_index : path)
	{
		grid[last_index] = grid[new_index];
		grid[new_index] = 0;
		last_index = new_index;
	
		for (int i = 0; i < grid.size() - 1; i++)
		{
			std::cout << grid[i] << " ";
			if (i % n == n - 1)
				std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

int	calculate_score(std::vector<int> grid)
{
	int move_needed = 0;

	for (int i = 0; i < SIZE*SIZE; i++)
	{
		int tile = grid[i];

		if (tile == 0 or tile == i + 1)
			continue;
		int x = i % SIZE;
		int y = i / SIZE;
		int x_target = (tile - 1) % SIZE;
		int y_target = (tile - 1) / SIZE;

		move_needed += abs(x - x_target) + abs(y - y_target);
	}
	return move_needed;
}

std::vector<int>	get_possible_moves(int index_blank, int size)
{
	std::vector<int>	index_swap;

	int x = index_blank % size;
	int y = index_blank / size;

	/*
 	* swap with under tile
	*/
	if (y < size - 1)
		index_swap.push_back((y + 1) * size + x);

	/*
 	* swap with upper title
	*/
	if (y > 0)
		index_swap.push_back((y - 1) * size + x);
	/*
 	* swap with right title
	*/
	if (x < size - 1)
		index_swap.push_back(y * size + x + 1);

	/*
 	* swap with left title
	*/
	if (x > 0)
		index_swap.push_back(y * size + x - 1);

	return index_swap;
}


std::vector<int>	search_algorithm(std::vector<int> grid)
{
	
	std::vector<int> winning_state = get_winning_grid(SIZE);
	std::set<std::vector<int>> visited;
	std::vector<int> init_path;
	init_path.push_back(find(grid.begin(), grid.end(),0) - grid.begin());

	auto cmp = [](std::pair<std::vector<int>, std::vector<int>> lhs, std::pair<std::vector<int>, std::vector<int>> rhs) { return lhs.first[SIZE] > rhs.first[SIZE]; };
	std::priority_queue<std::pair<std::vector<int>, std::vector<int>>, std::vector<std::pair<std::vector<int>, std::vector<int>>>, decltype(cmp)> toDo(cmp);
	
	toDo.push(std::make_pair(grid, init_path));

	int size = SIZE;

	while (!toDo.empty())
	{
		std::pair<std::vector<int>, std::vector<int>> current = toDo.top();
		toDo.pop();
		visited.insert(current.first);
		int index_blank = current.second.back();
	
		for (int new_blank : get_possible_moves(index_blank, size))
		{
			std::vector<int> tmp = current.first;
			std::vector<int> tmp_path = current.second;
			tmp_path.push_back(new_blank);			
			
			tmp[index_blank] = tmp[new_blank];
			tmp[new_blank] = 0;
			tmp[SIZE] = calculate_score(tmp);
			if (tmp[SIZE] == 0)
			{
				return tmp_path;
			}

			if (visited.find(tmp) == visited.end())
			{
				toDo.push(std::make_pair(tmp, tmp_path));
				continue ;
			}
		}
	}
	return init_path;
}
