#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

class State
{
	public:
		std::vector<int> grid;
		const State		*parent = NULL;
		const int	n = 0;
		int		move = 0;
		int		score = -1;

		State();
		State(std::vector<int> t_grid);
		State(std::vector<int> t_grid, const State* t_parent);
		
		bool	operator==(std::vector<int> cmp_grid);
		std::vector<int>	get_grid() const;
		int	find_blank() const;
		int	calculate_score();
		void	display_grid() const;
		std::vector<State>	get_possible_moves() const;
};

#endif
