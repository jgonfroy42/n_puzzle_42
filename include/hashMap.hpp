#include "npuzzle.hpp"
#include "State.hpp"


class HashMap
{
public:
	HashMap();
	~HashMap();
	HashMap(const HashMap & other);
	HashMap & operator=(const HashMap & other);

	State *&	operator[](const uint64_t & index);
	State *&	at(const uint64_t & index);
	State *&	insert(const uint64_t & index, State * val);
	bool		contains(const uint64_t & index);
	size_t		size() const {return this->_states_stored;};
	
private:
	State **	_buckets;
	size_t		_size;
	size_t		_states_stored = 0;
};
