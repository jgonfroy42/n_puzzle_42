#include "npuzzle.hpp"
#include "State.hpp"

/**
 * @brief an A* implementation that uses the State class
 * 
 * @param init_state the starting search state
 * @return a struct containing stats about the search and a path from start state to a winning state
 */
SearchResult a_star(State *init_state)
{
	SearchResult ret;
	State winning_state(get_winning_grid(init_state->getSideSize()));

	auto cmp = [](const State * lhs, const State * rhs) { return lhs->score + lhs->move >= rhs->score + rhs->move;};
	std::priority_queue<const State*, std::vector<const State *>, decltype(cmp)> toDo(cmp);

	std::unordered_set<State> visited;
	visited.reserve(1000000);

	toDo.push(init_state);
	ret.open_states++;
	visited.insert(*init_state);

	while(!toDo.empty())
	{
		const State *current = toDo.top();
		toDo.pop();
		ret.closed_states++;
		if (current->move > ret.max_depth)
			ret.max_depth = current->move;

		if (*current == winning_state)
		{
			ret.path = current->create_path();
			ret.success = true;
			ret.max_states_in_memory = visited.size();
			ret.max_transpositions = State::get_transpos_size();
			return ret;
		}

		for (State & move : current->get_possible_moves())
		{
			if (visited.find(move) == visited.end())
			{
				auto iter = (visited.insert(std::move(move))).first;
				const State * ptr = &(*iter);
				toDo.push(ptr);
				ret.open_states++;
				continue;
			}
		}
		// current->clear_grid();
		ret.iterations++;
	}
	ret.success = false;
	return ret;
}
