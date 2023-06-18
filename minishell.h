/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 17:19:34 by injah             #+#    #+#             */
/*   Updated: 2023/06/19 01:09:04 by bvaujour         ###   ########.fr       */
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
# include <dirent.h>

//couleurs normales texte
# define RESET			"\1\1\x1B[0m\2"
# define BLACK			"\1\x1B[30m\2"
# define RED			"\1\x1B[31m\2"
# define GREEN			"\1\x1B[32m\2"
# define YELLOW			"\1\x1B[33m\2"
# define BLUE			"\1\x1B[34m\2"
# define MAGENTA		"\1\x1B[35m\2"
# define CYAN			"\1\x1B[36m\2"
# define WHITE			"\1\x1B[37m\2"

//couleurs claires texte
# define C_BLACK		"\1\x1B[90m\2"
# define C_RED			"\1\x1B[91m\2"
# define C_GREEN		"\1\x1B[92m\2"
# define C_YELLOW		"\1\x1B[93m\2"
# define C_BLUE			"\1\x1B[94m\2"
# define C_MAGENTA		"\1\x1B[95m\2"
# define C_CYAN			"\1\x1B[96m\2"
# define C_WHITE		"\1\x1B[97m\2"

//couleurs grasses texte
# define BO_BLACK		"\1\x1B[1m\x1B[30m\2"
# define BO_RED			"\1\x1B[1m\x1B[31m\2"
# define BO_GREEN		"\1\x1B[1m\x1B[32m\2"
# define BO_YELLOW		"\1\x1B[1m\x1B[33m\2"
# define BO_BLUE		"\1\x1B[1m\x1B[34m\2"
# define BO_MAGENTA		"\1\x1B[1m\x1B[35m\2"
# define BO_CYAN		"\1\x1B[1m\x1B[36m\2"
# define BO_WHITE		"\1\x1B[1m\x1B[37m\2"

//couleurs normales background
# define BG_BLACK		"\1\x1B[40m\2"
# define BG_RED			"\1\x1B[41m\2"
# define BG_GREEN		"\1\x1B[42m\2"
# define BG_YELLOW		"\1\x1B[43m\2"
# define BG_BLUE		"\1\x1B[44m\2"
# define BG_BLUE		"\1\x1B[44m\2"
# define BG_MAGENTA		"\1\x1B[45m\2"
# define BG_CYAN		"\1\x1B[46m\2"
# define BG_WHITE		"\1\x1B[47m\2"

//couleurs claires background
# define BGC_BLACK		"\1\x1B[1m\x1B[100m\2"
# define BGC_RED		"\1\x1B[1m\x1B[101m\2"
# define BGC_GREEN		"\1\x1B[1m\x1B[102m\2"
# define BGC_YELLOW		"\1\x1B[1m\x1B[103m\2"
# define BGC_BLUE		"\1\x1B[1m\x1B[104m\2"
# define BGC_MAGENTA	"\1\x1B[1m\x1B[105m\2"
# define BGC_CYAN		"\1\x1B[1m\x1B[106m\2"
# define BGC_WHITE		"\1\x1B[1m\x1B[107m\2"

//couleurs grasses background
# define BGO_BLACK		"\1\x1B[1m\x1B[40m\2"
# define BGO_RED		"\1\x1B[1m\x1B[41m\2"
# define BGO_GREEN		"\1\x1B[1m\x1B[42m\2"
# define BGO_YELLOW		"\1\x1B[1m\x1B[43m\2"
# define BGO_BLUE		"\1\x1B[1m\x1B[44m\2"
# define BGO_MAGENTA	"\1\x1B[1m\x1B[45m\2"
# define BGO_CYAN		"\1\x1B[1m\x1B[46m\2"
# define BGO_WHITE		"\1\x1B[1m\x1B[47m\2"

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
	int	tmp;
}	t_fd;

typedef struct s_data
{
	char cwd[PATH_MAX];
	char	**paths;
	char	**env;
	char	**ghost;
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
	
	int	last_pid;
	short	step;

	int	lit;
	int	d_lit;
	int	slash;
	
}	t_data;

char	*wildcards(t_data *data, char *cmd);
void	end(t_data *data, char *ret);
char	*get_env(t_data *data, char *macro);
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
void	signals(t_data *data, int sig);
void	handler_1(int sig);
void	handler_2(int sig);
void	handler_back_slash(int sig);
void	edit_dollar(t_data *data);
char	*manage_exit(t_data *data, char *cmd);

void	free_all(t_data *data);

#endif