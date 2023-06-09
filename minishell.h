/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 17:19:34 by injah             #+#    #+#             */
/*   Updated: 2023/06/09 11:13:52 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/wait.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <limits.h>

# define RESET   "\x1B[0m"
# define BLACK   "\x1B[30m"
# define RED     "\x1B[31m"
# define GREEN   "\x1B[32m"
# define YELLOW  "\x1B[33m"
# define BLUE    "\x1B[34m"
# define MAGENTA "\x1B[35m"
# define CYAN    "\x1B[36m"
# define WHITE   "\x1B[37m"

# define BO_BLACK   "\x1B[1m\x1B[30m"
# define BO_RED     "\x1B[1m\x1B[31m"
# define BO_GREEN   "\x1B[1m\x1B[32m"
# define BO_YELLOW  "\x1B[1m\x1B[33m"
# define BO_BLUE    "\x1B[1m\x1B[34m"
# define BO_MAGENTA "\x1B[1m\x1B[35m"
# define BO_CYAN    "\x1B[1m\x1B[36m"
# define BO_WHITE   "\x1B[1m\x1B[37m"

# define BG_BLACK   "\x1B[40m"
# define BG_RED     "\x1B[41m"
# define BG_GREEN   "\x1B[42m"
# define BG_YELLOW  "\x1B[43m"
# define BG_BLUE    "\x1B[44m"
# define BG_MAGENTA "\x1B[45m"
# define BG_CYAN    "\x1B[46m"
# define BG_WHITE   "\x1B[47m"

# define BGO_BLACK   "\x1B[1m\x1B[40m"
# define BGO_RED     "\x1B[1m\x1B[41m"
# define BGO_GREEN   "\x1B[1m\x1B[42m"
# define BGO_YELLOW  "\x1B[1m\x1B[43m"
# define BGO_BLUE    "\x1B[1m\x1B[44m"
# define BGO_MAGENTA "\x1B[1m\x1B[45m"
# define BGO_CYAN    "\x1B[1m\x1B[46m"
# define BGO_WHITE   "\x1B[1m\x1B[47m"

typedef struct s_data
{
	char	**paths;
	char	**env;
    char    **new_env;
	char	*cur_dir;
	char	*prompt;
	
	int	base_fd[2];
	char	**cmd;
	char cwd[PATH_MAX];
	int	p_fd[2];
	int	pipe;
	int	child;
}	t_data;

void	recoded(t_data *data, char *cmd, int option);
void	end_process(t_data *data);
void	ft_free_tab(char **tab);
void	close_n_dup(t_data *data);
void    new_envi(t_data *data, char *str);
char	*parse_export(char *input);

#endif