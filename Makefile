# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/03 10:07:27 by jgraf             #+#    #+#              #
#    Updated: 2025/01/07 15:53:26 by nmonzon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#					COMPILATION INFORMATION
CC = cc
FLAGS = -Wall -Wextra -Werror -Iinclude
NAME = minishell

#					SOURCE AND OBJECT FILES
LIB = libft/libft.a
SRC_DIR = src
MAIN_SRC_DIR = src/main
FUNCTIONALITY_SRC_DIR = src/functionality
TOKENS_SRC_DIR = src/tokens
UTILS_SRC_DIR = src/utils

SOURCE = main.c \
         create_tokens.c \
         ft_tokensplit.c \
         ft_tokentrim.c \
         validation.c \
         error_handling.c \
         execution.c \
         utils.c

MAIN_SRC := $(addprefix $(MAIN_SRC_DIR)/, main.c)
FUNCTIONALITY_SRC := $(addprefix $(FUNCTIONALITY_SRC_DIR)/, error_handling.c validation.c execution.c)
TOKENS_SRC := $(addprefix $(TOKENS_SRC_DIR)/, create_tokens.c ft_tokensplit.c ft_tokentrim.c)
UTILS_SRC := $(addprefix $(UTILS_SRC_DIR)/, utils.c)

SRC_FILES = $(MAIN_SRC) $(FUNCTIONALITY_SRC) $(TOKENS_SRC) $(UTILS_SRC)
OBJ = $(SRC_FILES:.c=.o)

#					MAKEFILE CODE

all: $(LIB) $(NAME)

$(LIB):
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
	$(MAKE) -C libft fclean

re: fclean all

.PHONY: all clean fclean re