NAME = npuzzle

TEST_MAIN = main_test.cpp

MAIN = main.cpp

SRCS =	Config.cpp \
		State.cpp

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -g3

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

test

-include $(DEPS)

.PHONY: all clean flean re test vtest