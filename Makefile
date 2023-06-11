# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/06 16:33:49 by vanitas           #+#    #+#              #
#    Updated: 2023/06/11 01:44:50 by bvaujour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		=	minishell

CC			=	cc

FLAG		=	-Wall -Wextra -Werror -g3

VFLAGS		=	valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all --show-reachable=no --track-fds=yes

LIBFT_PATH	=	./libft/

LIBFT_FILE	=	libft.a

LIBFT_LIB	=	$(addprefix $(LIBFT_PATH), $(LIBFT_FILE))

SRC		=	main.c					\
			command.c				\
			utils.c					\
			signals.c				\
			
OBJ			=	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJ) minishell.h Makefile
	$(CC) $(OBJ) $(LIBFT_LIB) -o $(NAME) -lreadline

boot: $(NAME)
	clear
	./$(NAME)

valgrind: $(NAME)
	$(VFLAGS) ./$(NAME)
	
	
.c.o:
	@$(CC) $(FLAG) -c $< -o $@

$(LIBFT_LIB):
	@make -C $(LIBFT_PATH)

clean:
	@make clean -sC $(LIBFT_PATH)
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_PATH)

re: fclean all

.PHONY: all clean fclean re