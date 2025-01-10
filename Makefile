# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/03 10:07:27 by jgraf             #+#    #+#              #
#    Updated: 2025/01/10 13:43:53 by nmonzon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#														COMPILATION INFORMATION
CC = cc
FLAGS = -Wall -Wextra -Werror -Iinclude
NAME = minishell

#					SOURCE AND OBJECT FILES
LIB = libft/libft.a
SRC_DIR = src
MAIN_SRC_DIR = src/main
SHELL_SRC_DIR = src/shell
TOKENS_SRC_DIR = src/tokens
UTILS_SRC_DIR = src/utils
SIGNAL_SRC_DIR = src/signals
BUILTIN_SRC_DIR = src/builtins

MAIN_SRC := $(addprefix $(MAIN_SRC_DIR)/, main.c)
SHELL_SRC := $(addprefix $(SHELL_SRC_DIR)/, validation.c execution.c)
TOKENS_SRC := $(addprefix $(TOKENS_SRC_DIR)/, create_tokens.c ft_tokensplit.c ft_tokentrim.c)
UTILS_SRC := $(addprefix $(UTILS_SRC_DIR)/, utils.c execution_utils.c error_handling.c)
SIGNAL_SRC := $(addprefix $(SIGNAL_SRC_DIR)/, signal_handler.c)
BUILTIN_SRC := $(addprefix $(BUILTIN_SRC_DIR)/, cd.c env.c export.c unset.c echo.c execute_buildin.c pwd.c)

SRC_FILES = $(MAIN_SRC) $(SHELL_SRC) $(TOKENS_SRC) $(UTILS_SRC) $(SIGNAL_SRC) $(BUILTIN_SRC)
OBJ = $(SRC_FILES:.c=.o)

#					MAKEFILE CODE

all: $(LIB) $(NAME)

$(LIB):
	rm -rf libft
	git clone https://github.com/N03l-MG/libft.git libft
	$(MAKE) -C libft

$(NAME): $(OBJ) $(LIB)
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

.PHONY: all clean fclean re