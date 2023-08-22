# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/06 16:33:49 by vanitas           #+#    #+#              #
#    Updated: 2023/08/22 17:09:22 by mablatie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		=	minishell

CC			=	cc

FLAG		=	-Wall -Wextra -Werror -g

VFLAGS		=	valgrind --trace-children=yes -s --leak-check=full --show-leak-kinds=all --suppressions=/home/bvaujour/projets/mini/readline.supp --track-origins=yes --track-fds=yes

#VFLAGS		=	valgrind --leak-check=full --track-fds=yes --trace-children=yes

LIBFT_PATH	=	./libft/

LIBFT_FILE	=	libft.a

LIBFT_LIB	=	$(addprefix $(LIBFT_PATH), $(LIBFT_FILE))

SRC		=	main.c						\
			src/header.c				\
			src/command.c				\
			src/test.c					\
			src/process.c				\
			src/parsing.c				\
			src/init.c					\
			src/end.c					\
			src/utils.c					\
			src/utils2.c				\
			src/utils3.c				\
			src/parse_operator.c		\
			src/parse_operator2.c		\
			src/stock_struct.c			\
			src/stock_op_cmd.c			\
			src/stock_in.c				\
			src/stock_out.c				\
			src/stock_s_cmd.c			\
			src/execution.c				\
			src/execution2.c			\
			src/redirection.c			\
			src/redirection2.c			\
			src/env.c					\
			src/env2.c					\
			src/echo.c					\
			src/cd.c					\
			src/exit.c					\
			src/exit2.c					\
			src/export.c				\
			src/signals.c				\
			src/dollar.c				\
			src/wildcards.c				\
			src/wildcards2.c			\
			src/lit.c					\
			src/bash_morphing.c			\
						
OBJ			=	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJ)
	$(CC) $(OBJ) $(LIBFT_LIB) -o $(NAME) -lreadline

boot: $(NAME)
	@./$(NAME)

valgrind: $(NAME)
	$(VFLAGS) ./$(NAME)
	
push	: fclean
	git add *
	git commit -m 'auto'
	git push
	
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
