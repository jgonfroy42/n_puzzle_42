#include "npuzzle.hpp"
#include <fstream>
#include <sstream>

#define UNDEFINED -1
#define SPACE 0
#define NO_SPACE 1


int	check_space_line(std::vector<int>	&grid, std::string line)
{
	int style = UNDEFINED;

	while (!line.empty())
	{
		std::string token = line.substr(0, 2);
		line = line.substr(2);
		if (token[0] == ' ')
			style = SPACE;
		if (token[1] == ' ')
		{
			std::cerr << "Wrong format file" << std::endl;
		}	
	}

	return style;
}

void	check_no_space_line(std::string line)
{
	for (int i = 0; i < line)
	{
		if 
	}
}

void	parsing(std::string path)
{
	std::ifstream file(path);

	int n = -1;
	int x = 0;
	int y = 0;
	int style = UNDEFINED;
	bool comment = false;
	std::vector<int> grid;

	if (file.is_open())
	{
		std::string line;
		while (file)
		{
			std::string parsed_line;

			getline(file, line);
			line = line.substr(0,line.find('#'));
			if (line.empty())
				continue ;
			if (n == -1)
			{
				if (line.find_first_not_of("0123456789") != std::string::npos)
				{
					std::cerr << "Wrong format file" << std::endl;
					exit(1);
				}
				n = stoi(line);
				if (n == 3)
					style = NO_SPACE;
				if (n > 100)
				{
					std::cerr << "n too big" << std::endl;
					exit(1);
				}
				continue ;
			}
			if (line.find_first_not_of(" 0123456789") != std::string::npos || line.size() > 2*n+(n-1))
			{
				std::cerr << "Wrong format file" << std::endl;
				exit(1);
			}

			if (style != NO_SPACE)
			{
				if (line.size() < 2*n+(n-1))
				{
					if (style == SPACE)
					{
						std::cerr << "wrong format file" << std::endl;
						exit(1);
					}
					if (style == UNDEFINED)
						style == NO_SPACE;					
				}
				else
				{
					style = check_space_line(grid, line);
					continue ;
				}
			}
			check_no_space_line();
		}
		std::cout << "read all file" << std::endl;
		file.close();

	}
}

int main()
{
	parsing("test.txt");
}
