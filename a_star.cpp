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

	auto cmp = [](State * lhs, State * rhs) { return lhs->score >= rhs->score;};
	std::priority_queue<State*, std::vector<State *>, decltype(cmp)> toDo(cmp);

	std::unordered_map<uint64_t, State> visited;

	toDo.push(init_state);
	ret.open_states++;
	visited.insert(std::make_pair(init_state->get_hash(), *init_state));

	while(!toDo.empty())
	{
		State *current = toDo.top();
		toDo.pop();
		ret.closed_states++;

		if (*current == winning_state)
		{
			ret.path = current->create_path();
			ret.success = true;
			ret.max_states_in_memory = visited.size();
			return ret;
		}

		for (State & move : current->get_possible_moves())
		{
			if (visited.find(move.get_hash()) == visited.end())
			{
				auto iter = (visited.insert(std::make_pair(move.get_hash(), std::move(move)))).first;
				State * ptr = &iter->second;
				toDo.push(ptr);
				ret.open_states++;
				continue;
			}
		}
		ret.iterations++;
	}
	ret.success = false;
	return ret;
}
