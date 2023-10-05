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
		auto ret = deepening_search(*init_state, palier, winning_state, end_path, visited);
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

int deepening_search(State &state, int palier, State &winning_state, std::vector<State> & end_path, std::unordered_set<uint64_t> & visited)
{

	if (state.score > palier)
		return state.score;
	if (state == winning_state)
		return 0;

	int min = INT_MAX;
	// std::cout << "expanding state\n";
	// state.display_grid(); 
	// std::cout << std::endl;
	for (auto &move : state.get_possible_moves())
	{
		if (visited.contains(move.get_hash())) continue;
		visited.insert(move.get_hash());
		auto ret = deepening_search(move, palier, winning_state, end_path, visited);
		if (ret == 0)
		{
			if (end_path.empty())
				end_path = move.create_path();
			return 0;
		}
		if (ret < min)
			min = ret;
	} 
	return min;
}
