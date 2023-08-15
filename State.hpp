#pragma once
#include "npuzzle.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

class State
{
	public:
		grid_format grid;
		const State	*parent = NULL;
		// const int	n = 0;
		int		move = 0;
		int		score = -1;

		State();
		State(grid_format t_grid);
		State(grid_format t_grid, const State* t_parent);

		static int getSize();
		// void setSize(const int & n);
		
		bool	operator==(grid_format cmp_grid);
		bool operator==(const State & other);
		grid_format	get_grid() const;
		int	find_blank() const;
		int	calculate_score();
		void	display_grid() const;
		std::vector<State>	get_possible_moves() const;
	private:
		static int n;

};
