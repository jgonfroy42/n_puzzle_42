#include "npuzzle.hpp"
#include "State.hpp"

SearchResult	search_algorithm(State *init_state)
{
	int palier = init_state->score;
	SearchResult search;
	State winning_state(get_winning_grid(State::getSideSize()));
	std::vector<State> end_path;
	std::unordered_map<uint64_t, int> visited; //hash and depth


//créer le chemin dans la fonction init sans avoir besoin de retour ?	
	while (true)
	{
		// visited.clear();
		end_path.clear();
		end_path.push_back(*init_state);
		int ret = deepening_search(palier, 0, winning_state, end_path, visited, search);
		if (ret == INT_MAX)
			break;
		if (ret == 0)
		{
			search.path = end_path;
			search.success = true;
			search.max_transpositions = State::get_transpos_size();
			return search;
		}
		palier = ret;
		std::cout << "increasing threshold to :" << palier << std::endl;
	}

	search.success = false;
	return search;
}

int deepening_search(int palier, int g, State &winning_state, std::vector<State> & end_path, std::unordered_map<uint64_t, int> & visited, SearchResult & search)
{
	State & state = end_path.back();

	search.iterations++;
	search.open_states++;
	int f = g + state.score;
	if (g > search.max_depth)
	{
		search.max_depth = g;
		search.max_states_in_memory = g;
	}
	if ( f > palier)
		return f ;
	if (state == winning_state)
		return 0;

	int min = INT_MAX;
	// std::cout << "expanding state\n";
	// state.display_grid(); 
	// std::cout << std::endl;
	for (auto &move : state.get_possible_moves())
	{
		// auto iter = visited.find(move.get_hash());
		// if (iter == visited.end() || iter->second > g)
		// 	visited.insert(std::make_pair(move.get_hash(), g));
		// else if (iter->second < g)
		// 	continue;
		if (move.has_been_visited()) continue;
		end_path.push_back(std::move(move));
		int ret = deepening_search(palier, g + 1, winning_state, end_path, visited, search);
		if (ret == 0)
			return 0;
		if (ret < min)
			min = ret;
		end_path.pop_back();
	}
	return min;
}
