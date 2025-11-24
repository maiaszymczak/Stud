NAME		= btc
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -Iincludes

SRCS		= main.cpp \
			  src/BitcoinExchange.cpp

OBJS		= $(SRCS:.cpp=.o)

HEADERS		= includes/BitcoinExchange.hpp \
			  includes/Colors.hpp

GREEN		= \033[0;32m
RED			= \033[0;31m
RESET		= \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

%.o: %.cpp $(HEADERS)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "$(GREEN)✓ Compiled: $<$(RESET)"

clean:
	@rm -f $(OBJS)
	@echo "$(RED)✗ Object files removed$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)✗ $(NAME) removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re
