#include "npuzzle.hpp"
#include "State.hpp"

SearchResult	search_algorithm(State *init_state)
{
	int palier = init_state->score;
	SearchResult search;
	State winning_state(get_winning_grid(State::getSideSize()));
	std::vector<State> end_path;
	std::unordered_set<uint64_t> visited;

//créer le chemin dans la fonction init sans avoir besoin de retour ?	
	while (true)
	{
		visited.clear();
		end_path.clear();
		end_path.push_back(*init_state);
		int ret = deepening_search(palier, 0, winning_state, end_path, visited);
		if (ret == 0)
		{
			search.path = end_path;
			search.success = true;
			return search;
		}
		palier = ret;
		std::cout << "increasing threshold to :" << palier << std::endl;
	}

	search.success = false;
	return search;
}

int deepening_search(int palier, int g, State &winning_state, std::vector<State> & end_path, std::unordered_set<uint64_t> & visited)
{
	State & state = end_path.back();
	int f = g + state.score;
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
		// if (visited.count(move.get_hash()))
		// 	continue;;
		// visited.insert(move.get_hash());
		end_path.push_back(std::move(move));
		int ret = deepening_search(palier, g + 1, winning_state, end_path, visited);
		if (ret == 0)
			return 0;
		if (ret < min)
			min = ret;
		end_path.pop_back();
	}
	return min;
}
