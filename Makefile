# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/06 16:33:49 by vanitas           #+#    #+#              #
#    Updated: 2023/09/22 18:30:40 by mablatie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN		=	\e[92;2;118m
YELLOW		=	\e[93;4;226m
GRAY		=	\e[33;2;37m
RESET		=	\e[0m
CURSIVE		=	\e[33;3m

NAME		=	minishell

CC			=	cc

FLAG		=	-Wall -Wextra -Werror

VFLAGS		=	valgrind --trace-children=yes -s --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes

LIBFT_PATH	=	./libft/

LIBFT_FILE	=	libft.a

LIBFT_LIB	=	$(addprefix $(LIBFT_PATH), $(LIBFT_FILE))

SRC		=	main.c						\
			src/header.c				\
			src/command.c				\
			src/process.c				\
			src/parsing.c				\
			src/init.c					\
			src/init2.c					\
			src/end.c					\
			src/utils.c					\
			src/utils2.c				\
			src/utils3.c				\
			src/utils4.c				\
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
			src/extra.c					\
			src/extra2.c				\
						
OBJ			=	$(SRC:.c=.o)

DEP			=	$(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJ)
	@$(CC) $(OBJ) $(LIBFT_LIB) -o $(NAME) -lreadline
	@echo -n "                                                        \r"
	@echo "$(GREEN)Compilation complete\r$(RESET)"


boot: $(NAME)
	@./$(NAME)

valgrind: $(NAME)
	$(VFLAGS) ./$(NAME)
	
push: fclean
	git add *
	git commit -m 'auto'
	git push

.c.o:
	@$(CC) $(FLAG) -c $< -o $@
	@printf "$(CURSIVE)$(GRAY)Compiling Minishell files.....%-25s \r" $@

$(LIBFT_LIB):
	@echo "$(YELLOW)Preparing Minishell$(RESET)$(CURSIVE)$(GRAY)"
	@make -sC $(LIBFT_PATH)

clean:
	@make clean -sC $(LIBFT_PATH)
	@rm -f $(DEP)
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)
	@make fclean -sC $(LIBFT_PATH)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
