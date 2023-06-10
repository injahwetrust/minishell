/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 17:19:34 by injah             #+#    #+#             */
/*   Updated: 2023/06/10 17:18:04 by bvaujour         ###   ########.fr       */
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
# define HEADER		 "    __  ____       _      __         ____\n\
   /  |/  (_)___  (_)____/ /_  ___  / / /\n\
  / /|_/ / / __ \\/ / ___/ __ \\/ _ \\/ / / \n\
 / /  / / / / / / (__  ) / / /  __/ / /  \n\
/_/  /_/_/_/ /_/_/____/_/ /_/\\___/_/_/   \n"
#define	HEADER2		"MMMMMMMM               MMMMMMMM  iiii                     iiii                  hhhhhhh                                lllllll lllllll \n\
M:::::::M             M:::::::M i::::i                   i::::i                 h:::::h                                l:::::l l:::::l \n\
M::::::::M           M::::::::M  iiii                     iiii                  h:::::h                                l:::::l l:::::l \n\
M:::::::::M         M:::::::::M                                                 h:::::h                                l:::::l l:::::l \n\
M::::::::::M       M::::::::::Miiiiiiinnnn  nnnnnnnn    iiiiiii     ssssssssss   h::::h hhhhh           eeeeeeeeeeee    l::::l  l::::l \n\
M:::::::::::M     M:::::::::::Mi:::::in:::nn::::::::nn  i:::::i   ss::::::::::s  h::::hh:::::hhh      ee::::::::::::ee  l::::l  l::::l \n\
M:::::::M::::M   M::::M:::::::M i::::in::::::::::::::nn  i::::i ss:::::::::::::s h::::::::::::::hh   e::::::eeeee:::::eel::::l  l::::l \n\
M::::::M M::::M M::::M M::::::M i::::inn:::::::::::::::n i::::i s::::::ssss:::::sh:::::::hhh::::::h e::::::e     e:::::el::::l  l::::l \n\
M::::::M  M::::M::::M  M::::::M i::::i  n:::::nnnn:::::n i::::i  s:::::s  ssssss h::::::h   h::::::he:::::::eeeee::::::el::::l  l::::l \n\
M::::::M   M:::::::M   M::::::M i::::i  n::::n    n::::n i::::i    s::::::s      h:::::h     h:::::he:::::::::::::::::e l::::l  l::::l \n\
M::::::M    M:::::M    M::::::M i::::i  n::::n    n::::n i::::i       s::::::s   h:::::h     h:::::he::::::eeeeeeeeeee  l::::l  l::::l \n\
M::::::M     MMMMM     M::::::M i::::i  n::::n    n::::n i::::i ssssss   s:::::s h:::::h     h:::::he:::::::e           l::::l  l::::l \n\
M::::::M               M::::::Mi::::::i n::::n    n::::ni::::::is:::::ssss::::::sh:::::h     h:::::he::::::::e         l::::::ll::::::l\n\
M::::::M               M::::::Mi::::::i n::::n    n::::ni::::::is::::::::::::::s h:::::h     h:::::h e::::::::eeeeeeee l::::::ll::::::l\n\
M::::::M               M::::::Mi::::::i n::::n    n::::ni::::::i s:::::::::::ss  h:::::h     h:::::h  ee:::::::::::::e l::::::ll::::::l\n\
MMMMMMMM               MMMMMMMMiiiiiiii nnnnnn    nnnnnniiiiiiii  sssssssssss    hhhhhhh     hhhhhhh    eeeeeeeeeeeeee llllllllllllllll\n"
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

void	recoded(t_data *data, char *cmd);
void	end_process(t_data *data);
void	ft_free_tab(char **tab);
void	close_n_dup(t_data *data);
void    add_in_env(t_data *data, char *str);
char	*parse_export(char *input);
char	*parse_unset(char *input);
void	cd_manage(t_data *data, char *cmd);
int		manage_nonchild(t_data *data, char *input);
void	edit_pipe(t_data *data, char *input);
void	edit_prompt(t_data *data, char *cwd);
void	edit_paths(t_data *data);
char	*ez_money(t_data *data, char *cmd);

void	signals(int sig);
void	handler_1(int sig);
void	handler_2(int sig);
void	handler_back_slash(int sig);

void	free_all(t_data *data, char *input);

#endif