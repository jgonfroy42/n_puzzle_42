#pragma once
#include "npuzzle.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>

enum direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};
class State
{
	public:
		// grid_format grid;
		optimized_grid grid = NULL;
		const State	*parent = NULL;
		// const int	n = 0;
		int		move = 0;
		int		score = -1;
		direction dir = NONE;

		State();
		~State();
		State(const State & other);
		State(grid_format t_grid);
		// State(const std::vector<int> & grid);
		// State(grid_format t_grid, const State* t_parent);

		static int getSideSize();
		static int getTotalSize();
		static int getTotalStates();
		static void setSize(const int & n);
		// void setSize(const int & n);
		void setTargetPosition();


		bool	operator==(grid_format cmp_grid) const;
		bool operator==(const State & other) const ;
		bool operator<(const State & other) const;
		State & operator=(const State & other);
		optimized_grid	get_grid() const;
		uint64_t get_hash() const;
		int	find_blank() const;
		int	calculate_score();
		int calculate_linear_colision();
		void	display_grid() const;
		void	display_dir() const;
		void	clear_grid();
		std::vector<State>	get_possible_moves() const;
		std::vector<State>		create_path() const;
	private:
		//constructor to call when generating children
		State(const State * parent, int index_blank, int index_swap, direction dir);
		static size_t total_states;
		static int	n; //size of the side of the puzzle
		static int	size; //number of cells in the puzzle
		static std::vector<int>	target_position; //pos of tile of winning grid

		/* HASH related stuff */
		static std::vector<std::vector<uint64_t>> hash_grid;

		static std::unordered_map<uint64_t, int> transposition_table;

		uint64_t	hash;


		void			calculate_hash(int index_blank, int index_swap);
		void			calculate_start_hash();
		void	generate_hash_grid();
};
