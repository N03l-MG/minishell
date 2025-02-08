# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/03 10:07:27 by jgraf             #+#    #+#              #
#    Updated: 2025/02/06 19:58:08 by nmonzon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# This project was done in collaboration with Jacob Graf (jgraf)
# The work was split between us and the project would be nowhere without his help.
# Check out his github: https://github.com/Cimex404

#					COMPILATION INFORMATION
CC = cc
FLAGS = -Wall -Wextra -Werror -Iinclude #-g -fsanitize=address
NAME = minishell

GREEN = \033[0;32m
RESET = \033[0m

#					SOURCE AND OBJECT FILES
LIB = libft/libft.a
SRC_DIR = src
ENV_SRC_DIR = src/env_manipulation
PARSING_SRC_DIR = src/parsing
TOKEN_SRC_DIR = src/tokenizer
UTILS_SRC_DIR = src/utils
SIGNAL_SRC_DIR = src/signals
BUILTIN_SRC_DIR = src/builtins
EXEC_SRC_DIR = src/execution
ERROR_SRC_DIR = src/error_cleanup

EXEC_SRC := $(addprefix $(EXEC_SRC_DIR)/, execution.c execution_utils.c execute_builtin.c \
                                        redirections.c heredoc.c processes.c)
ENV_SRC := $(addprefix $(ENV_SRC_DIR)/, env_prechanges.c init_env.c update_env.c)
TOKENS_SRC := $(addprefix $(TOKEN_SRC_DIR)/, token_lexer.c token_trim.c token_utils.c)
PARSING_SRC := $(addprefix $(PARSING_SRC_DIR)/, get_env_variables.c validation.c)
UTILS_SRC := $(addprefix $(UTILS_SRC_DIR)/, misc_utils.c path_construction.c)
SIGNAL_SRC := $(addprefix $(SIGNAL_SRC_DIR)/, signal_handler.c)
BUILTIN_SRC := $(addprefix $(BUILTIN_SRC_DIR)/, cd.c env.c export.c unset.c echo.c pwd.c)
ERROR_SRC := $(addprefix $(ERROR_SRC_DIR)/, error_handling.c cleanup.c)

SRC_FILES = src/main.c $(EXEC_SRC) $(ENV_SRC) $(TOKENS_SRC) $(PARSING_SRC) \
            $(UTILS_SRC) $(SIGNAL_SRC) $(BUILTIN_SRC) $(ERROR_SRC)
OBJ = $(SRC_FILES:.c=.o)

#					MAKEFILE CODE

all: $(LIB) $(NAME)

$(LIB):
	$(MAKE) -C libft

$(NAME): $(OBJ) $(LIB)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIB) -lreadline
	@printf "$(GREEN)Successfully compiled: $(RESET)%s\n" $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "$(GREEN)Compiling: $(RESET)%s\n" $(notdir $<)
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@$(MAKE) -C libft clean
	@printf "$(GREEN)Cleaned object files$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@rm libft/libft.a
	@printf "$(GREEN)Cleaned everything$(RESET)\n"

re: fclean all

# Some special rules I added for automatic object cleaning. Makes testing easier.
.NOTPARALLEL:
.FORCE:

reclean: re .FORCE
	$(MAKE) clean

allclean: all .FORCE
	$(MAKE) clean

.PHONY: all clean fclean re reclean allclean .FORCE