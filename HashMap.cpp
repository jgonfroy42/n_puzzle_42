#include "hashMap.hpp"

HashMap::HashMap()
{
	//an approximation for 8gb of state storage
	// this->_size = 8000000000 / (State::getTotalSize() * sizeof(cell_size));
	//roughly 1 billion states
	this->_size = 10000000 * (State::getTotalSize() * sizeof(cell_size));
	std::cerr << "HashMap size is :" << this->_size << std::endl;
	this->_buckets = new State *[this->_size];
	std::memset(this->_buckets, '\0', this->_size * sizeof(State *));
}

HashMap::~HashMap()
{
	for(size_t i =0; i < this->_size; i++)
	{
		if (this->_buckets[i] != NULL)
			delete this->_buckets[i];
	}
	delete [] this->_buckets;
}

HashMap::HashMap(const HashMap & other)
{
	*this = other;
}

HashMap & HashMap::operator=(const HashMap & other)
{
	this->_size = other._size;
	if(this->_buckets)
		delete [] this->_buckets;
	this->_buckets = new State *[this->_size];
	std::memcpy(this->_buckets, other._buckets, this->_size);

	return *this;
}

State *& HashMap::operator[](const uint64_t & index)
{
	return this->_buckets[index % _size];
}

State *& HashMap::at(const uint64_t & index)
{
	return this->_buckets[index % _size];
}

State *& HashMap::insert(const uint64_t & index, State * val)
{
	if (this->_buckets[index % _size] != NULL)
		std::cerr << "COLLISION !!!!!\n";
	else
		this->_states_stored++;
	this->_buckets[index % _size] = val;
	return this->_buckets[index % _size];
}

bool	HashMap::contains(const uint64_t & index)
{
	return this->_buckets[index % _size] != NULL;
}
