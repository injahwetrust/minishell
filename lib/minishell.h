/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 17:19:34 by injah             #+#    #+#             */
/*   Updated: 2023/09/22 14:29:25 by mablatie         ###   ########.fr       */
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
# include "../libft/libft.h"
# include <limits.h>
# include <signal.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <termios.h>
//couleurs normales texte
# define RESET			"\1\x1B[0m\2"
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
# define CURSIVE		"\e[33;3m"
# define GRAY			"\e[33;2;37m"

# define HEADER1	 	"    __  ____       _      __         ____\n"
# define HEADER2		"   /  |/  ( )___  ( )____/ /_  ___  / / /\n"
# define HEADER3		"  / /|_/ / / __ \\/ / ___/ __ \\/ _ \\/ / /\n"
# define HEADER4		" / /  / / / / / / (__  ) / / /  __/ / /\n"
# define HEADER5		"/_/  /_/_/_/ /_/_/____/_/ /_/\\___/_/_/"
# define HEADER6		" injah/vanitas"
//Cd Error for norm
# define CD_ERR_CHDIR " getcmd: cannot access parent directories: %s\n"
# define CD_ERR_1 "Minishell: cd: %s: %s\n"

//For data->ex
# define DATA_EX "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"

//Parse Errors
# define PARSE_ERR_SYCH "Minishell: Syntax error near unexpected token « %c »\n"
# define PARSE_ERR_SYCH_2 " Syntax error near unexpected token « %c%c »\n"
# define PARSE_ERR_SYCH_3 " Syntax error near unexpected token « %c%c%c »\n"
# define PARSE_ERR_SYSTR " Syntax error near unexpected token « %s »\n"
# define RBRACK_ERR_1 "Minishell: Syntax error near unexpected token « < »\n"
# define RBRACK_ERR_2 "Minishell: Syntax error near unexpected token « << »\n"
# define RBRACK_ERR_3 "Minishell: Syntax error near unexpected token « <<< »\n"
# define BRACK_ERR_1 "Minishell: Syntax error near unexpected token « > »\n"
# define BRACK_ERR_2 "Minishell: Syntax error near unexpected token « >> »\n"
# define CPAR_ERR "Minishell: Syntax error near unexpected token « ) »\n"
# define EXPORT_ERR "Minishell: export: « %s » not a valid identifier\n"

//HereDoc ERR
# define H_ERROR " Warning « heredoc » terminated by EOF (instead of « %s »)\n"

//Norm for this LIB
# define MINI "Minishell:"
# define CHDIR "chdir: error retrieving current directory:"
# define AZ_MIN "abcdefghijklmnopqrstuvwxyz"
# define COUNT_S "count space:"

typedef struct s_fd
{
	int	base_fd[2];
	int	p_fd[2];
}	t_fd;

typedef struct s_cmd
{
	char	*cmd;
	char	**s_cmd;
	char	**in;
	char	**out;
	char	*prev_op;
	char	*next_op;
}	t_cmd;

typedef struct s_data
{
	char		cwd[PATH_MAX];
	char		*input;
	char		*save;
	char		*last_cmd;
	char		**paths;
	char		**env;
	char		**ghost;
	char		**new_env;
	char		*cur_dir;
	char		*prompt;
	char		*ex;
	t_cmd		*cmds;
	t_fd		fd;
	int			last_pid;
	short		step;
	short		print;
	short int	lit;
	short int	d_lit;
	int			par;
	int			in;
	int			out;
	int			and;
	int			or;
	int			count;
	int			argc;
	int			active_ret;
	int			step1;
	int			heredoc_ret;
}	t_data;

extern int	g_last_ret;

void		header(void);
int			replace_in_env(t_data *data, char *str);
int			info(t_data *data);
int			process(t_data *data);
void		end(t_data *data);
void		init(t_data *data, char **argv, char **env);
int			init_loop(t_data *data);
void		edit_lit(t_data *data, char c);
void		edit_par(t_data *data, char c);
void		end_loop(t_data *data);
int			parse_input(t_data *data);
int			parse_op(t_data *data);
int			remove_tab(t_data *data, char *str);
void		stock(t_data *data);
int			in_charset(char c, char *charset);
void		fill_cmd(t_data *data);
void		fill_op(t_data *data);
void		init_cmds(t_data *data);
int			is_lit(t_data *data);
void		remove_str(char *src, char *str);
void		fill_in(t_data *data);
void		fill_out(t_data *data);
int			mixed_op(char *cmd, char c);
char		*get_env(t_data *data, char *macro);
void		execution(t_data *data);
char		*get_exec(char *cmd, t_data *data);
void		edit_paths(t_data *data, char *cmd);
void		print(void);
void		step0(t_data *data);
void		fill_s_cmd(t_data *data);
int			redirection(t_data *data, t_cmd *ccmd);
void		creation(t_data *data);
void		constant_built_in(t_data *data, char **s_cmd);
int			echo(char **s_cmd);
int			env(t_data *data, char **s_cmd);
void		canceled_built_in(t_data *data, char **s_cmd);
int			active_built_in(t_data *data, char **s_cmd);
int			cd(t_data *data, char **s_cmd);
int			add_in_env(t_data *data, char *str);
int			remove_from_env(t_data *data, char *str);
int			is_in_env(t_data *data, char *str);
int			parse_export(t_data *data, char *input);
void		signals(int sig);
void		manage_dollar(t_data *data);
char		*wildcards(t_data *data);
int			cont(t_data *data, char c);
char		*seg_dir(t_data *data, char *new, int i);
char		*get_ast(t_data *data, char *input);
int			false_space(t_data *data);
void		manage_last_cmd(t_data *data);
void		manage_lit(t_data *data);
int			manage_exit(t_data *data, char **s_cmd);
void		bash_morphing(t_data *data);
int			print_declare(t_data *data);
int			op_newline_norm(t_data *data, int i);
void		cancel_cmd_norm(char **s_cmd);
void		creation(t_data *data);
int			join_in_env(t_data *data, char *str);
void		underflow(t_data *data, char *cmd, char *max);
void		checkflow(t_data *data, char *cmd, char *max, int i);
void		dir_null(t_data *data, DIR *dir, char **s_cmd);
int			error_print(char *s_cmd);
void		step1(t_data *data);
void		redirect_close(t_data *data);
void		check_ghost(t_data *data, char *str);
void		remove_from_ghost(t_data *data, char *str);
void		toggle_signals_off(void);
void		toggle_signals_on(void);
int			print_history(void);
int			history(t_data *data);
void		exec_norm(t_data *data, int i);
void		close_and_dup_child(t_data *data);
int			init_env(t_data *data, char **env);
void		init_norm(t_data *data, char **env);
int			save_cmd(t_data *data, char **s_cmd);
void		cmd_print(void);
void		boot_history(void);
int			cmd_choice(t_data *data);
int			delete_cmd(t_data *data, char **s_cmd);
char		*str_dollar(t_data *data, char *str);
char		*manage_dollar_heredoc(t_data *data, char *here);

#endif