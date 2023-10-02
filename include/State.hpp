#pragma once
#include "npuzzle.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>

class State
{
	public:
		// grid_format grid;
		optimized_grid grid = NULL;
		const State	*parent = NULL;
		// const int	n = 0;
		int		move = 0;
		int		score = -1;

		State();
		~State();
		State(const State & other);
		State(grid_format t_grid);
		// State(const std::vector<int> & grid);
		// State(grid_format t_grid, const State* t_parent);

		static int getSideSize();
		static int getTotalSize();
		static void setSize(const int & n);
		// void setSize(const int & n);
		
		bool	operator==(grid_format cmp_grid);
		bool operator==(const State & other);
		State & operator=(const State & other);
		optimized_grid	get_grid() const;
		int	find_blank() const;
		int	calculate_score();
		int calculate_linear_colision();
		void	display_grid() const;
		std::vector<State>	get_possible_moves() const;
	private:
		//constructor to call when generating children
		State(const State * parent, int index_blank, int index_swap);
		static int n; //size of the side of the puzzle
		static int size; //number of cells in the puzzle

};
