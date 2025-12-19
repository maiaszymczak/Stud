NAME = ircserv

CXX = c++
CXXFLAGS += -Wall -Wextra -Werror -std=c++98 -MMD -MP
CXXFLAGS += -g3

SOURCES_DIR = srcs/
INCLUDE_DIR = include/
BUILD_DIR = build/

# Colors
RESET = \033[0m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
CYAN = \033[0;36m
BOLD_GREEN = \033[1;32m
BOLD_YELLOW = \033[1;33m
BOLD_CYAN = \033[1;36m

SOURCE_FILES = \
	 main.cpp \
	 Channel.cpp \
	 Client.cpp \
	 Command.cpp \
	 Connection.cpp \
	 Fd.cpp \
	 Message.cpp \
	 Nickname.cpp \
	 Response.cpp \
	 Server.cpp \
	 User.cpp \
	 utils.cpp \
	 Epoll.cpp

SRCS = $(addprefix $(SOURCES_DIR), $(SOURCE_FILES))
OBJS = $(addprefix $(BUILD_DIR), ${SRCS:.cpp=.o})
DEPS = $(OBJS:.o=.d)

IFLAGS += -I$(INCLUDE_DIR)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BOLD_CYAN) Linking $(NAME)...$(RESET)"
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "$(BOLD_GREEN)✓ $(NAME) created successfully!$(RESET)"

$(BUILD_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "$(CYAN) Compiling $<...$(RESET)"
	@$(CXX) $(CXXFLAGS) $(IFLAGS) -c -o $@ $<

clean:
	@echo "$(YELLOW) Cleaning object files...$(RESET)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)✓ Clean complete!$(RESET)"

fclean: clean
	@echo "$(YELLOW) Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)✓ Full clean complete!$(RESET)"

re: fclean
	@echo "$(BOLD_YELLOW) Rebuilding everything...$(RESET)"
	@$(MAKE) all

-include $(DEPS)

.PHONY: all bonus clean fclean re
