#pragma once
#include "npuzzle.hpp"
#include <fstream>
#include <map>
#include <cstring>

enum config_error
{
	ALL_CLEAR,
	WRONG_FORMAT,
	INVALID_CHAR,
	MISSING_INFO,
	UNSUPPORTED_SIZE,
	NO_SUCH_FILE,
	INCOMPLETE_GRID,
};

class Config
{
public:
	Config();
	Config(const std::string & filename);
	~Config();

	std::string			getFileName(void) const {return this->_file_name;};
	int					getGridSize(void) const {return this->_grid_size;};
	std::vector<int>	getGrid(void) const {return this->_grid;};
	e_algo				getAlgo(void) const {return this->_algo;};
	eval				getEval(void) const {return this->_e_eval;};
	Search_params		getSearchParams(void) const {return this->_search_params;};
	bool				isLoaded() const {return this->_loaded;};

	bool				setAlgo(std::string & algo_name);
	bool				setHeuristic(std::string & heuristic_name);
	bool				setSearchParams(std::string & search_name);
	bool		loadNewFile(const std::string & filename);

	void		printError(void) const;
	void		clear(void);

private:
	std::vector<int> _grid;
	size_t 			_grid_size = 0;
	std::ifstream	_config_file;
	size_t			_current_line = 0;
	std::string		_file_name;
	e_algo			_algo = IDA_STAR;
	eval			_e_eval;
	Search_params  _search_params;
	bool			_loaded = false;

	//error handling
	config_error error_type;
	int			last_errno = 0;

	bool	_readSize();
	bool	_readGrid();
	bool	_readConfig();
};
