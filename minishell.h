/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 17:19:34 by injah             #+#    #+#             */
/*   Updated: 2023/06/15 19:27:24 by bvaujour         ###   ########.fr       */
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
# include <signal.h>
#include <dirent.h>

# define RESET			"\1\x1B[0m\2"
# define BLACK			"\x1B[30m"
# define RED			"\x1B[31m"
# define GREEN			"\x1B[32m"
# define YELLOW			"\x1B[33m"
# define BLUE			"\x1B[34m"
# define MAGENTA		"\x1B[35m"
# define CYAN			"\x1B[36m"
# define WHITE			"\x1B[37m"

# define BO_BLACK		"\1\x1B[1m\x1B[30m\2"
# define BO_RED			"\1\x1B[1m\x1B[31m\2"
# define BO_GREEN		"\1\x1B[1m\x1B[32m\2"
# define BO_YELLOW		"\x1B[1m\x1B[33m"
# define BO_BLUE		"\x1B[1m\x1B[34m"
# define BO_MAGENTA		"\x1B[1m\x1B[35m"
# define BO_CYAN		"\x1B[1m\x1B[36m"
# define BO_WHITE		"\x1B[1m\x1B[37m"

# define BG_BLACK		"\x1B[40m"
# define BG_RED			"\x1B[41m"
# define BG_GREEN		"\1\x1B[42m\2"
# define BG_YELLOW		"\x1B[43m"
# define BG_BLUE		"\x1B[44m"
# define BG_MAGENTA		"\x1B[45m"
# define BG_CYAN		"\x1B[46m"
# define BG_WHITE		"\x1B[47m"

# define BGO_BLACK		"\x1B[1m\x1B[40m"
# define BGO_RED		"\x1B[1m\x1B[41m"
# define BGO_GREEN		"\x1B[1m\x1B[42m"
# define BGO_YELLOW		"\x1B[1m\x1B[43m"
# define BGO_BLUE		"\x1B[1m\x1B[44m"
# define BGO_MAGENTA	"\x1B[1m\x1B[45m"
# define BGO_CYAN		"\x1B[1m\x1B[46m"
# define BGO_WHITE		"\x1B[1m\x1B[47m"
# define HEADER1	 	"    __  ____       _      __         ____\n"
# define HEADER2		"   /  |/  (_)___  (_)____/ /_  ___  / / /\n"
# define HEADER3		"  / /|_/ / / __ \\/ / ___/ __ \\/ _ \\/ / /\n"
# define HEADER4		" / /  / / / / / / (__  ) / / /  __/ / /\n"
# define HEADER5		"/_/  /_/_/_/ /_/_/____/_/ /_/\\___/_/_/\n"

typedef struct s_fd
{
	int	base_fd[2];
	int	p_fd[2];
	int	redir_fd[2];
	int	heredoc;
	int	append;
}	t_fd;
typedef struct s_data
{
	char cwd[PATH_MAX];
	char	**paths;
	char	**env;
    char    **new_env;
	char	*cur_dir;
	char	*prompt;
	char	**cmd;
	
	t_fd	fd;
	
	char	*ex;
	char	*wrong_char;
	
	short	last_ret;
	char	*input;
	
	int	pipe;
	int	dollar;
	
	short	step;
	
}	t_data;

void	end(t_data *data, char *ret);
void	print(t_data *data);
void	recoded(t_data *data, char *cmd);
void	end_process(t_data *data, char *ret);
void	ft_free_tab(char **tab);
void	close_n_dup(t_data *data);
void    add_in_env(t_data *data, char *str);
char	*parse_export(t_data *data, char *input);
char	*parse_unset(char *input);
void	cd_manage(t_data *data, char *cmd);
int		manage_nonchild(t_data *data);
void	edit_pipe(t_data *data);
void	edit_prompt(t_data *data, char *cwd);
void	edit_paths(t_data *data);
char	*ez_money(t_data *data);
int		in_ex(t_data *data, char c);
char	*redir_in(t_data *data, char *cmd);
char	*redir_out(t_data *data, char *cmd);
void	execution(t_data *data);
char	*get_exec(char *cmd, t_data *data);
int		still_in(char *cmd);
int		still_out(char *cmd);
void	signals(int sig);
void	handler_1(int sig);
void	handler_2(int sig);
void	handler_back_slash(int sig);
void	edit_dollar(t_data *data);

void	free_all(t_data *data);

#endif