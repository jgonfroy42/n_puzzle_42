#include "npuzzle.hpp"

std::vector<int>	get_target_position(int size)
{
	std::vector<int> targetPosition;
	grid_format target = get_winning_grid(size);

	targetPosition.resize(size * size);
	
	for (int i = 0; i < size * size; i++)
		targetPosition[target[i]] = i;

	return targetPosition;
} 

grid_format	get_winning_grid(int size)
{
	grid_format grid;
	grid.resize(size * size);

	char dir = 'E';
	int x = -1;
	int y = 0;
	int x_limit = size - 1;
	int y_limit = size - 1;

	int nb_tiles = size * size; 
	int num = 0;
	while (1)
	{
		while (dir == 'E')
		{
			x++;
			num++;

			//return grid if we check the end
			if (num == nb_tiles)
			{
				grid[y * size + x] = 0;
				return grid;
			}	
			
			grid[y * size + x] = num;

			//change direction if we reach a border or an already fill tile
			if (x == x_limit)
 				dir = 'S';
		}
	
		while (dir == 'S')
		{
			y++;
			num++;

			//return grid if we check the end
			if (num == nb_tiles)
			{
				grid[y * size + x] = 0;
				return grid;
			}

			grid[y * size + x] = num;

			//change direction if we reach a border or an already fill tile
			if (y == y_limit)
				dir = 'W';
		}

		while (dir == 'W')
		{
			x--;
			num++;

			//return grid if we check the end
			if (num == nb_tiles)
			{
				grid[y * size + x] = 0;
				return grid;
			}	

			grid[y * size + x] = num;

			//change direction if we reach a border or an already fill tile
			if (x == size - x_limit -1)
			{
				dir = 'N';
				x_limit--;
			}
		}

		while (dir == 'N')
		{
			y--;
			num++;

			//return grid if we check the end
			if (num == nb_tiles)
			{
				grid[y * size + x] = 0;
				return grid;
			}	

			grid[y * size + x] = num;

			//change direction if we reach a border or an already fill tile
			if (y == size - y_limit)
			{
				dir = 'E';
				y_limit--;
			}
		}
	}

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
	std::vector<int> target_position = get_target_position(nsize);

	for (size_t i = 0; i < grid.size() - 1; i++)
	{
		if (grid[i] == 0)
			blank_index = i;
		for (size_t j = i + 1; j < grid.size(); j++)
			if (target_position[grid[i]] > target_position[grid[j]])
				inversion++;
	}
 
	//if blank distant to position is odd, solvable if number of inversion is odd
	//if distant is even, number of inversion must be even too

	int blank_dist = abs(target_position[0] / nsize - blank_index / nsize) + abs(target_position[0] % nsize - blank_index % nsize);

	return blank_dist % 2 == inversion % 2;
}
