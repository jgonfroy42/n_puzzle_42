#include "npuzzle.hpp"

uint64_t generate_random_uint64(void)
{
	return ((uint64_t)std::rand() << 48) | ((uint64_t)std::rand() << 32) | ((uint64_t)std::rand()) << 16 | (uint64_t)std::rand();
}

void remove_comments(std::string & line)
{
	//remove comments
	size_t pos = line.find('#');
	if (pos != line.npos)
		line.erase(pos);
}

bool isOnlyDigits(std::string & line)
{
	auto iter = std::find_if_not(line.begin(), line.end(), ::isdigit);

	return iter == line.end();
}

bool isOnlyDigit_and_space(std::string & line)
{
	auto iter = std::find_if_not(line.begin(), line.end(), [](const int & c)
	{
		return std::isdigit(c) || std::isspace(c);
	});

	return iter == line.end();
}
