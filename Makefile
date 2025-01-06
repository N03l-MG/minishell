# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/03 10:07:27 by jgraf             #+#    #+#              #
#    Updated: 2025/01/06 16:02:23 by nmonzon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#														COMPILATION INFORMATION
CC = cc
FLAGS = -Wall -Wextra -Werror -Iinclude
NAME = minishell

#														SOURCE AND OBJECT FILES
LIB = libft/libft.a
SRC_DIR = src

SOURCE = create_tokens.c ft_tokensplit.c main.c validation.c error_handling.c utils.c
MAIN_SRC := $(addprefix $(SRC_DIR)/, $(SOURCE))
OBJ = $(MAIN_SRC:.c=.o)

#														MAKEFILE CODE
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