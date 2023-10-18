#include "Config.hpp"

Config::~Config()
{
	this->clear();
}

Config::Config()
{

}

Config::Config(const std::string & filename)
:_file_name(filename)
{
	this->loadNewFile(filename);
}

bool Config::setSearchParams(std::string & search_name)
{
	if (search_name == "UNIFORM") {
		this->_search_params = Search_params::UNIFORM_COST;
	} else if (search_name == "GREEDY") {
		this->_search_params = Search_params::GREEDY;
	} else
		return false;
	return true;
}

bool Config::setHeuristic(std::string & heuristic_name)
{
	if (heuristic_name == "MH") {
		this->_e_eval = eval::MANHATTAN;
	} else if (heuristic_name == "MH+LC") {
		this->_e_eval = eval::HYBRID;
	} else if (heuristic_name == "MP") {
		this->_e_eval = eval::MISSPLACED_TILES;
	} else
		return false;
	return true;
}

bool Config::setAlgo(std::string & algo_name)
{
	if (algo_name == "IDA_STAR")
	{
		this->_algo = IDA_STAR;
	} else if (algo_name == "A_STAR") {
		this->_algo = A_STAR;
	} else
		return false;
	return true;
}

bool Config::loadNewFile(const std::string & filename)
{
	this->clear();
	this->_config_file.open(filename);

	if (this->_config_file.fail())
	{
		last_errno = errno;
		return false;
	}

	//file reading

	//first part is the grid size
	//then the grid
	//and then there is the different algorithms choices

	if (!this->_readSize()) return false;
	if (!this->_readGrid()) return false;

	this->_loaded = true;
	return true;
}

void Config::clear(void)
{
	if (this->_config_file.is_open())
		this->_config_file.close();
	this->_grid_size = 0;
	this->_current_line = 0;
	this->_loaded = false;
}

bool Config::_readSize()
{
	if (!this->_config_file.is_open())
		return false;
	
	std::string line;
	while (std::getline(this->_config_file, line))
	{
		this->_current_line++;
		remove_comments(line);

		if (line.empty()) continue;
		if (!isOnlyDigits(line))
		{
			this->error_type = INVALID_CHAR;
			return false;
		}
		this->_grid_size = std::stoi(line);
		return true;
	}
	this->error_type = MISSING_INFO;
	return false;
}

bool Config::_readGrid()
{
	if (!this->_config_file.is_open())
	{
		this->error_type = NO_SUCH_FILE;
		return false;
	}

	std::string line;
	this->_grid.resize(this->_grid_size * this->_grid_size);

	size_t i = 0;
	size_t j = 0;
	while (std::getline(this->_config_file, line))
	{
		this->_current_line++;
		remove_comments(line);
		if (line.empty()) continue;

		if(!isOnlyDigit_and_space(line))
		{
			this->error_type = INVALID_CHAR;
			return false;			
		}

		size_t pos = 0;
		j = 0;
		while(!line.empty())
		{
			int tile = std::stoi(line, &pos);
			this->_grid[i] = tile;
			j++;
			i++;

			line.erase(0, pos);
			while(isspace(line[0]))
				line.erase(line.begin());
		
			if (i > this->_grid_size * this->_grid_size)
			{
				this->error_type = INVALID_LINE;
				return false;
			}
		}
		if (j < this->_grid_size)
		{
			this->error_type = INCOMPLETE_GRID;
			return false;
		}
	}
	if (i < this->_grid_size * this->_grid_size)
	{
		this->error_type = INCOMPLETE_GRID;
		return false;
	}

	grid_format copy = _grid;
	std::sort(copy.begin(), copy.end());

	for (int i = 0; i < (int)copy.size(); i++)
	{
		if (copy[i] != i)
		{
			this->error_type = WRONG_VALUES;
			return false;
		}
	}

	return true;
}

static std::map<config_error, std::string> error_to_str = {
	{ALL_CLEAR, "No error detected"},
	{WRONG_FORMAT, "Invalid file format"},
	{INVALID_CHAR, "Invalid char detected"},
	{MISSING_INFO, "Config file is incomplete"},
	{UNSUPPORTED_SIZE, "Grid size is not supported"},
	{NO_SUCH_FILE, "There is no such file"},
	{INCOMPLETE_GRID, "The grid is missing some tiles"},
	{INVALID_LINE, "There is too many number in line"},
	{WRONG_VALUES, "Values on the tiles are incorrect"}
};

void Config::printError(void) const
{
	std::cerr << "Config file error: ";
	if (_current_line != 0)
	{
		std::cerr << "line [" << _current_line << "]: ";
		std::cerr << error_to_str[error_type] << std::endl;
	}
	else
	{
		std::cerr << strerror(this->last_errno) << '\n';
	}
}
