#include "State.hpp"

		State::State() {}

		State::State(std::vector<int> t_grid) : grid(t_grid), n(sqrt(grid.size()))
		{
			this->score = calculate_score();
		}

		State::State(std::vector<int> t_grid, State *t_parent) : grid(t_grid), n(sqrt(grid.size())), parent(t_parent), move(t_parent->move + 1)
		{
			this->score = calculate_score();
		}

bool		operator==(State const& lhs, State const& rhs)
{
	return lhs.grid == rhs.grid;
}

bool		State::operator==(std::vector<int> cmp_grid)
{
	return this->grid == cmp_grid;
}

/*--------getter & setter-------*/

std::vector<int>	State::get_grid() const { return this->grid;}

/*--------methode-----------*/

int	State::find_blank() const
{
	return (find(this->grid.begin(), this->grid.end(),0) - this->grid.begin());
}

int	State::calculate_score()
{
	int move_needed = 0;

	for (int i = 0; i < this->grid.size(); i++)
	{
		int tile = this->grid[i];

		if (tile == 0 or tile == i + 1)
			continue;
		int x = i % n;
		int y = i / n;
		int x_target = (tile - 1) % n;
		int y_target = (tile - 1) / n;

		move_needed += abs(x - x_target) + abs(y - y_target);
	}
	//this->score = move_needed + move;  //get shortest path (g(x) + h(x) ?)
	this->score = move_needed;
	return this->score;
}

void	State::display_grid() const
{
	for (int i = 0; i < this->grid.size(); i++)
	{
		std::cout << this->grid[i] << " ";
		if (i % n == n - 1)
			std::cout << std::endl;
	}
}

State	*swap_tile(State *parent, int index_blank, int index_swap)
{
	std::vector<int> grid = parent->get_grid();

	grid[index_blank] = grid[index_swap];
	grid[index_swap] = 0;
	return new State(grid, parent);
}

std::vector<State*>	State::get_possible_moves()
{
	int		index_swap;
	std::vector<State *>	ret;

	int index = this->find_blank();
	int x = index % this->n;
	int y = index / this->n;

	/*
 	* swap with under tile
	*/
	if (y < n - 1)
	{
		index_swap = (y + 1) * n + x;	
		ret.push_back(swap_tile(this, index, index_swap));
	}

	/*
 	* swap with upper title
	*/
	if (y > 0)
	{
		index_swap = (y - 1) * n + x;	
		ret.push_back(swap_tile(this, index, index_swap));
	}

	/*
 	* swap with right title
	*/
	if (x < n - 1)
	{
		index_swap = y * n + x + 1;	
		ret.push_back(swap_tile(this, index, index_swap));
	}

	/*
 	* swap with left title
	*/
	if (x > 0)
	{
		index_swap = y * n + x - 1;	
		ret.push_back(swap_tile(this, index, index_swap));
	}
	return ret;
}
