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

enum eval
{
	manhattan,
	conflicts,
	hybrid,
};


/*
*	IMPORTANT POINTS ABOUT STATE CLASS
*
*	-It uses static variables to store things such a size that are common
*		to all instances so it is not possible to solve multiple puzzles at once
*		
*	- If you dont need to expand a node anymore you can delete its grid to optimize memory a bit
*	
*	- Every instances calculates its own hash that can be used in a hashmap by example
*
*	- it is possible to activate a transpoistion table to store every calculated score,
*		usefull if you are using a searching algo that goes a lot through the same states ( IDA * i see you )
*	
*	- This transposition table also stores the lowest depth a wich the state has been generated
		usefull if you need to know there is a better path for the current subtree
*/

/**
 * @brief a Class representing a state of the npuzzle
 * 
 * 
 */
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
		State & operator=(const State & other);
		// State(const std::vector<int> & grid);
		// State(grid_format t_grid, const State* t_parent);

		/**	STATIC FUNCTIONS **/

		/**
		 * @brief get the size of the side of the current puzzle size
		 * 
		 * @return int
		 */
		static int getSideSize();

		/**
		 * @brief Get the area of the current puzzle size
		 * 
		 * @return int 
		 */
		static int getTotalSize();

		/**
		 * @brief Get the number of states ever constructed
		 * 
		 * @return int 
		 */
		static int getTotalStates();

		/**
		 * @brief Set the size (side size format) of the state class
		 * this has effect on all instances.
		 * 
		 * @param n 
		 */
		static void setSize(const int & n);

		/**
		 * @brief Set the evaluation function for every States to use
		 * 
		 * @param eval_set
		 */
		static void		set_eval(eval eval_set);

		static size_t get_transpos_size();
		/**	OPERATORS **/
		// void setSize(const int & n);
		void setTargetPosition();


		bool	operator==(grid_format cmp_grid) const;

		/**
		 * @brief lexicographical equality comparison of States' grids
		 * 
		 * @param other 
		 * @return true if States have the same grid
		 * @return false if States dont have the same grid
		 */
		bool operator==(const State & other) const ;

		/**
		 * @brief lexicographical inferiority comparison of States' grids
		 * 
		 * @param other 
		 * @return true 
		 * @return false 
		 */
		bool operator<(const State & other) const;


		/** GETTERS & SETTERS **/

		/**
		 * @brief Get the underlying grid
		 * 
		 * @return a pointer to the grid
		 */
		optimized_grid	get_grid() const;

		/**
		 * @brief Get the zobrist hash of the grid,
		 * wich is automatically calculated at instanciation
		 * 
		 * @return a uint64 containing the current grid's hash
		 */
		uint64_t get_hash() const;

		/**
		 * @brief Uses the inner transposition table to return the lowest depth this state has been seen
		 * 
		 * @return int 
		 */
		int	get_lowest_depth() const;

		/**
		 * @brief Uses the inner transposition table to check if current node has already been seen before
		 * 
		 * @return true 
		 * @return false 
		 */
		bool has_been_visited() const;


		/** STATE LOGIC **/

		/**
		 * @brief Get a vector containing all possible next states of current states
		 * (children states)
		 * 
		 * @return std::vector<State> 
		 */
		std::vector<State>	get_possible_moves() const;

		/**
		 * @brief Create a path from head state to current state
		 * 
		 * @return std::vector<State> 
		 */
		std::vector<State>	create_path() const;



		/** UTILS **/

		/**
		 * @brief prints the underlying grid on std::cout
		 * 
		 */
		void	display_grid() const;
		/**
		 * @brief prints the move ( UP, DOWN etc...) used to transition to this state
		 * 
		 */
		void	display_dir() const;

		/**
		 * @brief free the grid's resources, to use only if you dont need to access the grid anymore.
		 * eg: state has already been expanded
		 */
		void	clear_grid();

	private:
		/**
		 * @brief Constructor that is only called by a parent State constructing its child
		 * 
		 * @param parent ptr to parent
		 * @param index_blank current index of blank tile
		 * @param index_swap current index of targeted move tile
		 * @param dir direction in wich the move is happening
		 */
		State(const State * parent, int index_blank, int index_swap, direction dir);


		static size_t total_states;//total number of states ever constructed
		static int	n; //size of the side of the puzzle
		static int	size; //number of cells in the puzzle
		static std::vector<int>	target_position; //pos of tile of winning grid

		/**
		 * @brief returns the index of the blank tile in the grid
		 * 
		 * @return int 
		 */
		int	find_blank() const;

		/**
		 * @brief calculates the score based on a heuristics function, only call this once at instanciation then refer to the "score" variable
		 * 
		 * @return int 
		 */
		int	calculate_score();

		/**
		 * @brief a heuristics function based on the linear collision/conflict widely known npuzzle heuristics
		 * 
		 * @return int 
		 */
		int calculate_linear_colision();
		/* HASH related stuff */

		/**
		 * @brief a double array containing all randomly generated hash used in the zobrist hash
		 * 
		 */
		static std::vector<std::vector<uint64_t>> hash_grid;

		/**
		 * @brief a transposition table containing all previously calculated scores
		 *  as well as the lowest depth a node has been seen
		 *  THE PAIR IS [SCORE, DEPTH]
		 */
		static std::unordered_map<uint64_t, std::pair<int, int>> transposition_table;

		uint64_t	hash;

		/**
		 * @brief calculate a new zobrist hash from a move
		 * 
		 * @param index_blank 
		 * @param index_swap 
		 */
		void			calculate_hash(int index_blank, int index_swap);

		/**
		 * @brief calculate initial zobrist hash, call only on initial state, then on every expansion call only calculate_hash
		 * 
		 */
		void			calculate_start_hash();

		/**
		 * @brief call once to generate the random positions hashes
		 */
		void	generate_hash_grid();
};

namespace std {
	template <>
	struct hash<State> {
		size_t operator()(const State & state) const {
			return (size_t(state.get_hash()));
		}
	};
} 
