#include "State.hpp"

State::State() {}

//first constructor called by main to init the start state
State::State(grid_format t_grid)
{
	State::total_states++;
	this->n = sqrt(t_grid.size());
	this->size = t_grid.size();
	this->grid = new cell_size[this->size];

	for(int i = 0; i < this->size; i++)
		this->grid[i] = t_grid[i];

	if (this->hash_grid.empty())
		this->generate_hash_grid();
	this->calculate_start_hash();
	this->score = calculate_score();
}

// State::State(const std::vector<int> & grid)
// {
// 	this->n = sqrt(grid.size());
// 	this->size = grid.size();
// 	this->grid = new cell_size[this->size];

// 	for(int i = 0; i < this->size; i++)
// 		this->grid[i] = grid[i];
	
// 	this->score = calculate_score();
// }

State::State(const State & other)
{
	*this = other;
}

//private constructor called when creating a child
State::State(const State * parent, int index_blank, int index_swap, direction dir)
{
	State::total_states++;
	this->grid = new cell_size[this->size];

	this->hash = parent->hash;
	std::memcpy(this->grid, parent->grid, this->size);
	calculate_hash(index_blank, index_swap);
	std::swap(this->grid[index_blank], this->grid[index_swap]);

	this->parent = parent;
	this->move = parent->move + 1;
	this->dir = dir;
	this->score = calculate_score();
	this->pos = parent->pos;
}

// State::State(grid_format t_grid, const State *t_parent)
// : grid(t_grid), parent(t_parent), move(t_parent->move + 1)
// {
// 	this->score = calculate_score();
// }

State::~State()
{
	delete[] this->grid;
}

State &		State::operator=(State const & other)
{
	if (!this->grid)
		this->grid = new cell_size[this->size];
	
	if (this->grid && other.grid)
		std::memcpy(this->grid, other.grid, this->size);

	this->score = other.score;
	this->parent = other.parent;
	this->move = other.move;
	this->dir = other.dir;
	this->hash = other.hash;

	return *this;
}

bool		State::operator==(State const& rhs) const
{
	for(int i = 0; i < this->size; i++)
	{
		if(this->grid[i] != rhs.grid[i])
			return false;
	}
	return true;
}

bool		State::operator<(const State & rhs) const
{
	for(int i = 0; i < this->size; i++)
	{
		if (this->grid[i] < rhs.grid[i])
			return true;
		else if (this->grid[i] > rhs.grid[i])
			return false;
	}
	return false;
}

bool		State::operator==(grid_format cmp_grid) const
{
	for(int i = 0; i < this->size; i++)
	{
		if (this->grid[i] != cmp_grid[i])
			return false;
	}
	return true;
}

/*--------getter & setter-------*/

optimized_grid	State::get_grid() const { return this->grid;}
uint64_t		State::get_hash() const {return this->hash;}

// void State::setSize(const int & n) { State::n = n;}

int State::getSideSize() { return State::n;}
int State::getTotalSize() {return State::size;}
int State::getTotalStates() {return State::total_states;}

void State::setSize(const int & n)
{
	State::n = n;
	State::size = n * n;
}

/*--------methode-----------*/

int	State::find_blank() const
{
	for(int i = 0; i < this->size; i++)
	{
		if (this->grid[i] == 0)
			return i;
	}
	return -1; //voluntary to create segfaults because there should never be no 0 in the grid
}

int	State::calculate_score()
{
	for (int pos : this->pos)
		std::cout << pos << std::endl;

	int move_needed = 0;

			auto iter = this->transposition_table.find(this->hash);
			if (iter != this->transposition_table.end())
			{
				this->score = iter->second;
				return this->score;
			}

	for (int i = 0; i < this->size; i++)
	{
		int tile = this->grid[i];

		if (tile == 0 or tile == i + 1)
			continue;
		int x = i % n;
		int y = i / n;
		int x_target = this->pos[tile] % n;
		int y_target = this->pos[tile] / n;

		move_needed += abs(x - x_target) + abs(y - y_target);
	}

	//this->score = move_needed + move;  //get shortest path (g(x) + h(x) ?)
	//this->score = move_needed;
	this->score = move_needed + (calculate_linear_colision());
	this->transposition_table.insert(std::make_pair(this->hash, this->score));
	return this->score;
}
//to qualify a linear collisions between two tiles ( a and b )
//they need to be on the same row or column
//they need to both have their goal position on the same row or column
//the goal position of one of the tiles is blocked by the other

//we check goal position of a tile by substracting one from it since tile number 1 will be at index 0
//(tiles are stored in a contiguous array for memory optimisations)
int State::calculate_linear_colision()
{
	int linear_collisions = 0;
	for(int col = 0; col < this->n; col++)
	{
		for(int row = 0; row < this->n; row++)
		{
			int tile_a = this->grid[row * n + col];
			if (tile_a == 0) continue;

			//if goal position of tile_a is in its current column
			if (tile_a % this->n == col)
			{
				//checking all others in same column ( so increasing row but not changing column yes its bizarre )
				for(int test_row = row + 1; test_row < this->n; test_row++)
				{
					int tile_b = this->grid[test_row * n + col];
					if (tile_b == 0) continue;
					
					//if target pos of tile_b is in current col and
					//either tile_a is target pos of tile_b or tile_b is target pos of tile_a
					//then there is a collision
					if (tile_b % this->n == col &&
						(	test_row * n + col == tile_a - 1
						||	row * n + col == tile_b - 1))
						linear_collisions++;
				}
			}
			
			//else if goal position of tile_a is in its curren row
			else if (tile_a / n == row)
			{
				//checking all others in same row ( so increasing column but not changing row )
				for(int test_col = col + 1; test_col < this->n; test_col++)
				{
					int tile_b = this->grid[row * n + test_col];
					if (tile_b == 0) continue;

					//if target pos of tile_b is in current row and
					//either tile_a is target pos of tile_b or tile_b is target pos of tile_a
					//then there is a collision
					if(tile_b / this->n == row &&
						(	row * n + test_col == tile_a - 1
						||	row * n + col == tile_b -1))
						linear_collisions++;
				}

			}
		}
	}
	return linear_collisions;
}

void	State::display_grid() const
{
	if (this->grid == NULL)
		throw std::exception();
	for (int i = 0; i < this->size; i++)
	{
		std::cout << std::setw(3) << int(this->grid[i]);
		if (i % n == n - 1)
			std::cout << std::endl;
	}
}

void State::display_dir() const
{
	switch (this->dir)
	{
		case UP: {
			std::cout << "UP\n";
			break;
		}case DOWN: {
			std::cout << "DOWN\n";
			break;
		}case LEFT: {
			std::cout << "LEFT\n";
			break;

		}case RIGHT:{
			std::cout << "RIGHT\n";
			break;
		} case NONE:{
			break;
		}
	}
}

void State::clear_grid()
{
	if (this->grid != NULL)
		delete [] this->grid;
	this->grid = NULL;
}

// State	swap_tile(const State *parent, int index_blank, int index_swap)
// {
// 	grid_format grid = parent->get_grid();

// 	grid[index_blank] = grid[index_swap];
// 	grid[index_swap] = 0;
// 	return State(grid, parent);
// }

std::vector<State>	State::get_possible_moves() const
{
	int		index_swap;
	std::vector<State>	ret;
	ret.reserve(4);

	int index = this->find_blank();
	int x = index % this->n;
	int y = index / this->n;

	/*
 	* swap with under tile
	*/
	if (y < n - 1)
	{
		index_swap = (y + 1) * n + x;	
		ret.push_back(State(this, index, index_swap, DOWN));
	}

	/*
 	* swap with upper title
	*/
	if (y > 0)
	{
		index_swap = (y - 1) * n + x;	
		ret.push_back(State(this, index, index_swap, UP));
	}

	/*
 	* swap with right title
	*/
	if (x < n - 1)
	{
		index_swap = y * n + x + 1;	
		ret.push_back(State(this, index, index_swap, RIGHT));
	}

	/*
 	* swap with left title
	*/
	if (x > 0)
	{
		index_swap = y * n + x - 1;	
		ret.push_back(State(this, index, index_swap, LEFT));
	}

	// std::shuffle(ret.begin(), ret.end(), rng_engine);
	return ret;
}

void	State::calculate_start_hash()
{
	this->hash = (std::rand() << sizeof(this->hash) / 2) |  std::rand();

	for(int i = 0; i < this->size; i++)
		this->hash ^= this->hash_grid[this->grid[i]][i];
}

void	State::calculate_hash(int index_blank, int index_swap)
{
	int value_swap = this->grid[index_swap];

	//removing 0 and swapping tiles from hash

	this->hash ^= this->hash_grid[0][index_blank];
	this->hash ^= this->hash_grid[value_swap][index_swap];

	//adding the new positions

	this->hash ^= this->hash_grid[0][index_swap];
	this->hash ^= this->hash_grid[value_swap][index_blank];
}

void	State::generate_hash_grid()
{
	if (State::size == 0) return;

	State::hash_grid.clear();
	State::hash_grid.resize(State::size);
	for(int i = 0; i < State::size; i++)
	{
		for(int j = 0; j < State::size; j++)
			State::hash_grid[i].push_back((std::rand() << sizeof(uint64_t) / 2) | (uint64_t)std::rand());
	}
}

std::vector<State> State::create_path() const
{
	std::cout << "creating path\n";
	std::stack<const State *> temp;
	std::vector<State> ret;
	const State * ptr = this;


	while (ptr != NULL)
	{
		temp.push(ptr);
		ptr = ptr->parent;
	}

	while (!temp.empty())
	{
		ret.push_back(*temp.top());
		temp.pop();
	}

	return ret;
}

void	State::setPosition()
{
	this->pos.reserve(size);

	for (int i = 0; i < this->getTotalSize(); i++)
		this->pos[this->grid[i]] = i;
}
