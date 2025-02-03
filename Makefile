# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/03 10:07:27 by jgraf             #+#    #+#              #
#    Updated: 2025/02/03 13:05:04 by nmonzon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#					COMPILATION INFORMATION
CC = cc
FLAGS = -Wall -Wextra -Werror -Iinclude
NAME = minishell

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
                                        execute_builtin_pipe.c redirections.c heredoc.c processes.c)
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
	rm -rf libft
	git clone https://github.com/N03l-MG/libft.git libft
	$(MAKE) -C libft

$(NAME): $(OBJ) $(LIB) # -fsanitize=address
	$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(LIB) -lreadline

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C libft clean

fclean: clean
	rm -f $(NAME)
	rm libft/libft.a

re: fclean all

.NOTPARALLEL:
.FORCE:

reclean: re .FORCE
	$(MAKE) clean

allclean: all .FORCE
	$(MAKE) clean

.PHONY: all clean fclean re reclean allclean .FORCE