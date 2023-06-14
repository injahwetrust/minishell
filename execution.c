/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 13:03:30 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/14 10:49:12 by bvaujour         ###   ########.fr       */
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
		errno = 3;
		perror(s_cmd[0]);
		ft_free_tab(data->cmd);
		ft_free_tab(s_cmd);
		ft_free_tab(data->paths);
		exit(127);
	}
}

void	exec(char *cmd, t_data *data)
{
	pid_t	pid;
	
	pid = fork();
	signals(2);
	if (pid == 0)
	{
		close(data->fd.redir_fd[1]);
		dup2(data->fd.redir_fd[0], 0);
		close(data->fd.redir_fd[0]);
		close_n_dup(data);
		recoded(data, cmd);
		go(cmd, data);
	}
	else
	{
		close(data->fd.redir_fd[0]);
		dup2(data->fd.redir_fd[1], 1);
		close(data->fd.redir_fd[1]);
		close(data->fd.p_fd[1]);
		dup2(data->fd.p_fd[0], 0);
		close(data->fd.p_fd[0]);
	}
}

void	execution(t_data *data)
{
	int	i;
	//int	j;
	
	i = -1;
	while (data->cmd[++i])
	{
		data->fd.redir_fd[0] = dup(0);
		data->fd.redir_fd[1] = dup(1);
		if (pipe(data->fd.p_fd) == -1)
			exit(ft_dprintf(2, "\xE2\x9A\xA0\xEF\xB8\x8F Pipe error\n")); // faire une fonction pour exit proprement
		data->step = 1;
		while (still_in(data->cmd[i]))
		{
			data->cmd[i] = redir_in(data, data->cmd[i]);
			if (!data->cmd[i] || !*data->cmd[i])
			{
				if (data->fd.redir_fd[0] != -1)
					close (data->fd.redir_fd[0]);
				break;
			}
		}
		if (!data->cmd[i] || !*data->cmd[i] || data->fd.redir_fd[0] == -1)
		{
			close(data->fd.p_fd[0]);
			close(data->fd.p_fd[1]);
			close(data->fd.redir_fd[1]);
			continue;
		}
		while (still_out(data->cmd[i]))
		{
			data->cmd[i] = redir_out(data, data->cmd[i]);
			if (!data->cmd[i] || !*data->cmd[i])
				break;
		}
		if (!data->cmd[i] || !*data->cmd[i] || data->fd.redir_fd[1] == -1)
		{
			close(data->fd.p_fd[1]);
			close(data->fd.p_fd[0]);
			close(data->fd.redir_fd[0]);
			continue;
		}
		if (ft_strcmp(data->cmd[i], "JOHNCARPENTER&DONALDDUCK") == 0)
		{
			close(data->fd.p_fd[0]);
			close(data->fd.p_fd[1]);
			close(data->fd.redir_fd[0]);
			close(data->fd.redir_fd[1]);
			return ;
		}
		data->cmd[i] = ft_strtrim(data->cmd[i], " \t", 1);
		exec(data->cmd[i], data);
		/*if (data->cmd[i + 1] && strncmp("echo", ft_strtrim(data->cmd[i + 1], " \t", 1), 4) == 0)
		{
			j = 0;
			while (j < 70000000)
				j++;
		}*/
		//trouver une maniere moins shlag de gerer le sigpipe de cat <infile | echo lol ou echo lol | cat <infile
	}
}
