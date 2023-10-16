NAME = npuzzle

SRCS =	main.cpp \
		Config.cpp \
		State.cpp \
		HashMap.cpp \
		grid_utils.cpp \
		a_star.cpp \
		ida_star.cpp \
		utils.cpp

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -Ofast

CPPFLAGS = -MMD -I ./include/

OBJS_DIR = ./objs/

OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

DEPS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.d))

all: $(NAME)
	@make $(NAME) -q && echo "All good here !"

$(OBJS_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: clean
	@make all

-include $(DEPS)

.PHONY: all clean flean re test vtest
