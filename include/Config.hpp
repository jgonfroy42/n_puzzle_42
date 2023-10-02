#include "npuzzle.hpp"
#include <fstream>
#include <map>

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

	std::string						getFileName(void) const {return this->_file_name;};
	int								getGridSize(void) const {return this->_grid_size;};
	std::vector<int>				getGrid(void) const {return this->_grid;};
	bool							isLoaded() const {return this->_loaded;};

	bool		loadNewFile(const std::string & filename);

	void		printError(void) const;
	void		clear(void);

private:
	std::vector<int> _grid;
	size_t 			_grid_size = 0;
	std::ifstream	_config_file;
	size_t			_current_line = 0;
	std::string		_file_name;
	bool			_loaded = false;

	//error handling
	config_error error_type;

	bool	_readSize();
	bool	_readGrid();
	bool	_readConfig();
};
