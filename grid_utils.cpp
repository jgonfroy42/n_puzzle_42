#include "npuzzle.hpp"

grid_format	get_winning_grid(int size)
{
	grid_format grid;

	for (cell_size i = 0; i < size*size; i++)
		grid.push_back(i + 1);
	grid[grid.size() - 1] = 0;

	return grid;
}

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
