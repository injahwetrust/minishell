/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vanitas <vanitas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:52:35 by injah             #+#    #+#             */
/*   Updated: 2023/06/06 16:42:28 by vanitas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

typedef struct s_data
{
	char	**paths;
	char	**env;
}	t_data;


void	ft_free_tab(char **tab)
{
	int	i;
	
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

char	*get_exec(char *cmd, t_data *data)
{
	int	i;
	char	*path;

	i = -1;
	while (data->paths[++i])
	{
		path = ft_strjoin(data->paths[i], cmd, 1);
		printf ("%s\n", path);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
	}
	return (cmd);
}

void	exec(char *cmd, t_data *data)
{
	char	**s_cmd;
	char	*path;
	pid_t	pid;
	
	pid = fork();
	if (pid == 0)
	{
		s_cmd = ft_split(cmd, ' ');
		path = get_exec(s_cmd[0], data);
		if (execve(path, s_cmd, data->env) <= -1)
		{
			errno = ESRCH;
			perror(s_cmd[0]);
			free(cmd);
			ft_free_tab(s_cmd);
			free(data->paths);
			exit(0);
		}
	}
	else
		waitpid(pid, NULL, 0);
}

char	**edit_paths(t_data *data, char **env)
{
    int     i;
    
    i = -1;
    data->paths = ft_split(getenv("PATH"), ':');
    while (data->paths[++i])
		data->paths[i] = ft_strjoin(data->paths[i], "/", 1);
	return (env);
}

int main(int ac, char **av, char **env) 
{
	t_data	data;
	char* input;
	char cwd[1024];
	char* trim;

	(void)ac;
	(void)av;
	edit_paths(&data, env);
	data.env = env;
	while (1) 
	{
		getcwd(cwd, sizeof(cwd));
		printf ("\033[1m\033[35m");
		input = readline(ft_strjoin(cwd, " \033[1m\033[32mMinishell\033[0m$ ", 0));
		if (input == NULL)
		{
			free(input);
			break;
		}
		trim = ft_strtrim(input, " \t");
		if (trim == NULL)
		 	continue;
		if (ft_strncmp("pwd", trim, 3) == 0)
			printf("%s\n", cwd);
		else
			exec(trim, &data);
		add_history(input);
		free(input);
		free(trim);
	}
	ft_free_tab(data.paths);
	return (0);
}
