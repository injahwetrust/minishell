/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:52:35 by injah             #+#    #+#             */
/*   Updated: 2023/06/08 22:16:17 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_exec(char *cmd, t_data *data)
{
	int	i;

	i = -1;
	while (data->paths[++i])
	{
		data->paths[i] = ft_strjoin(data->paths[i], cmd, 1);
		if (access(data->paths[i], F_OK | X_OK) == 0)
			return (data->paths[i]);
	}
	return (cmd);
}

void	go(char *cmd, t_data *data)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	path = get_exec(s_cmd[0], data);
	if (execve(path, s_cmd, data->env) <= -1)
	{
		ft_dprintf(2, "command not found: %s\n", s_cmd[0]);
		ft_free_tab(data->cmd);
		ft_free_tab(s_cmd);
		ft_free_tab(data->paths);
		exit(0);
	}
}

void	exec(char *cmd, t_data *data)
{
	pid_t	pid;
	
	pid = fork();
	if (pid == 0)
	{
		close_n_dup(data);
		recoded(data, cmd);
		go(cmd, data);
	}
	else
	{
		close(data->p_fd[1]);
		dup2(data->p_fd[0], 0);
		close(data->p_fd[0]);
	}
}

void	edit_paths(t_data *data)
{
    int     i;
    
    i = -1;
    data->paths = ft_split(getenv("PATH"), ':');
    while (data->paths[++i])
		data->paths[i] = ft_strjoin(data->paths[i], "/", 1);
}

void	edit_prompt(t_data *data, char *cwd)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	while (cwd[i])
		i++;
	while (cwd[i] != '/' && i > 0)
		i--;
	while (j < i)
	{
		cwd++;
		j++;
	}
	data->prompt = ft_strjoin(BG_GREEN BO_BLACK"Minishell~", getenv("USER"), 0);
	data->prompt = ft_strjoin(data->prompt, RESET BO_GREEN" ", 1);
	data->prompt = ft_strjoin(data->prompt, cwd, 1);
	data->prompt = ft_strjoin(data->prompt,RESET"$ ", 1);
}

void	execution(t_data *data)
{
	int	i;

	i = -1;
	while (data->cmd[++i])
	{
		if (pipe(data->p_fd) == -1)
			exit(0);
		data->cmd[i] = ft_strtrim(data->cmd[i], " \t", 1);
		ft_dprintf(2, "cmd %d = |%s|\n", i, data->cmd[i]);
		exec(data->cmd[i], data);
	}
}

void	print(t_data *data)
{
	int	ret;
	char	buff[50];
	
	(void)data;
	ret = 1;
	while (ret)
	{
		ret = read(0, buff, sizeof(buff));
		buff[ret] = '\0';
		ft_printf("%s", buff);
	}
	dup2(data->base_fd[0], 0);
	close (data->base_fd[0]);
	dup2(data->base_fd[1], 1);
	close (data->base_fd[1]);
}

void	init(t_data *data, char **env)
{
	data->env = env;
	edit_paths(data);
	data->pipe = 0;
}

int	main(int ac, char **av, char **env) 
{
	t_data	data;
	char* input = "env|grep lo";
	(void)ac;
	(void)av;
	
	init(&data, env);
	while (1) 
	{
		data.base_fd[0] = dup(0);
		data.base_fd[1] = dup(1);
		getcwd(data.cwd, sizeof(data.cwd));
		edit_prompt(&data, data.cwd);
		input = readline(data.prompt);
		if (!ft_strcmp(input, "exit"))
		{
			free(data.prompt);
			free(input);
			ft_free_tab(data.paths);
			close (data.base_fd[0]);
			close (data.base_fd[1]);
			exit(0);
		}
		if (input == NULL || !ft_strcmp(input, ""))
		{
			free(data.prompt);
			free(input);
			continue;
		}
		add_history(input);
		input = ft_strtrim(input, " \t", 1);
		data.cmd = ft_split(input, '|');
		free(input);
		free(data.prompt);
		execution(&data);
		while (wait(NULL) > 0)
			;
		print(&data);
		ft_free_tab(data.cmd);
	}
	//close (data.base_fd[0]);
	//close (data.base_fd[1]);
	//ft_free_tab(data.paths);
	return (0);
}
