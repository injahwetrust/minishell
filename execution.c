/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 13:03:30 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/28 14:59:12 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_exec(char *cmd, t_data *data)
{
	int	i;

	i = -1;
	if (!data->paths)
	return (cmd);
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
	DIR	*dir;
	
	edit_paths(data);
	s_cmd = ft_split(cmd, ' ');
	dir = opendir(s_cmd[0]);
	if (dir != NULL)
	{
		closedir(dir);
		ft_dprintf(2, "Minishell: %s: Is a directory\n", s_cmd[0]);
		end_process(data, "126");
	}
	path = get_exec(s_cmd[0], data);
	//dprintf(2, "path = %s\n", path);
	if (execve(path, s_cmd, data->env) <= -1)
	{
		errno = 3;
		perror(s_cmd[0]);
		ft_free_tab(data->cmd);
		ft_free_tab(data->ope);
		ft_free_tab(s_cmd);
		if (data->paths)
			ft_free_tab(data->paths);
		ft_free_tab(data->env);
		ft_free_tab(data->ghost);
		exit(127);
	}
}

void	exec_pipe(char *cmd, t_data *data)
{
	pid_t	pid;
	
	printf("exec_pipe  %s\n", cmd);
	if (pipe(data->fd.p_fd) == -1)
			exit(ft_dprintf(2, "\xE2\x9A\xA0\xEF\xB8\x8F Pipe error\n")); // faire une fonction pour exit proprement
	signals(data, 2);
	pid = fork();
	if (pid == 0)
	{
		//close_n_dup(data);
		close(data->fd.redir_fd[1]);
		if (!isatty(data->fd.redir_fd[0]))
			dup2(data->fd.redir_fd[0], 0);
		close(data->fd.redir_fd[0]);
		//close(data->fd.tmp);
		close(data->fd.base_fd[0]);
		close(data->fd.base_fd[1]);

		close(data->fd.p_fd[0]);
		dup2(data->fd.p_fd[1], 1);
		close(data->fd.p_fd[1]);
		recoded(data, cmd);
		go(cmd, data);
	}
	else
	{
		data->last_pid = pid;
		close(data->fd.redir_fd[0]);
		dup2(data->fd.redir_fd[1], 1);
		close(data->fd.redir_fd[1]);
		
		close(data->fd.p_fd[1]);
		dup2(data->fd.p_fd[0], 0);
		close(data->fd.p_fd[0]);
	}
}

void	exec_or(char *cmd, t_data *data)
{
	pid_t	pid;
	int	status;
	
	wait(&status);
	data->last_ret = WEXITSTATUS(status);
	if (!data->last_ret)
		return ;
	printf("exec_or  %s\n", cmd);
	if (pipe(data->fd.p_fd) == -1)
			exit(ft_dprintf(2, "\xE2\x9A\xA0\xEF\xB8\x8F Pipe error\n")); // faire une fonction pour exit proprement
	signals(data, 2);
	pid = fork();
	if (pid == 0)
	{
		//close_n_dup(data);
		close(data->fd.redir_fd[1]);
		if (!isatty(data->fd.redir_fd[0]))
			dup2(data->fd.redir_fd[0], 0);
		close(data->fd.redir_fd[0]);
		//close(data->fd.tmp);
		close(data->fd.base_fd[0]);
		close(data->fd.base_fd[1]);

		close(data->fd.p_fd[0]);
		dup2(data->fd.p_fd[1], 1);
		close(data->fd.p_fd[1]);
		recoded(data, cmd);
		go(cmd, data);
	}
	else
	{
		data->last_pid = pid;
		close(data->fd.redir_fd[0]);
		dup2(data->fd.redir_fd[1], 1);
		close(data->fd.redir_fd[1]);
		
		close(data->fd.p_fd[1]);
		dup2(data->fd.p_fd[0], 0);
		close(data->fd.p_fd[0]);
		// char	buf[50];
		// int	ret;
	
		// ret = 1;
		// while (ret)
		// {
		// 	ret = read(data->fd.p_fd[0], buf, sizeof(buf));
		// 	buf[ret] = '\0';
		// 	write(1, buf, ret);
		// }
		// close(data->fd.p_fd[0]);
		// close(data->fd.p_fd[1]);
	}
}

void	exec_and(char *cmd, t_data *data)
{
	pid_t	pid;
	int	status;
	
	waitpid(data->last_pid, &status, 0);
	data->last_ret = WEXITSTATUS(status);
	if (data->last_ret)
		return ;
	printf("exec_and  %s\n", cmd);
	if (pipe(data->fd.p_fd) == -1)
			exit(ft_dprintf(2, "\xE2\x9A\xA0\xEF\xB8\x8F Pipe error\n")); // faire une fonction pour exit proprement
	signals(data, 2);
	pid = fork();
	if (pid == 0)
	{
		//close_n_dup(data);
		close(data->fd.redir_fd[1]);
		if (!isatty(data->fd.redir_fd[0]))
			dup2(data->fd.redir_fd[0], 0);
		close(data->fd.redir_fd[0]);
		//close(data->fd.tmp);
		close(data->fd.base_fd[0]);
		close(data->fd.base_fd[1]);

		close(data->fd.p_fd[0]);
		dup2(data->fd.p_fd[1], 1);
		close(data->fd.p_fd[1]);
		recoded(data, cmd);
		go(cmd, data);
	}
	else
	{
		data->last_pid = pid;
		close(data->fd.redir_fd[0]);
		dup2(data->fd.redir_fd[1], 1);
		close(data->fd.redir_fd[1]);
		
		close(data->fd.p_fd[1]);
		dup2(data->fd.p_fd[0], 0);
		close(data->fd.p_fd[0]);
		// char	buf[50];
		// int	ret;
	
		// ret = 1;
		// while (ret)
		// {
		// 	ret = read(data->fd.p_fd[0], buf, sizeof(buf));
		// 	buf[ret] = '\0';
		// 	write(1, buf, ret);
		// }
		// close(data->fd.p_fd[0]);
		// close(data->fd.p_fd[1]);
	}
}

void	execution(t_data *data)
{
	int	i;
	i = -1;

	while (data->cmd[++i])
	{
		data->fd.redir_fd[0] = dup(data->fd.base_fd[0]);
		data->fd.redir_fd[1] = dup(data->fd.base_fd[1]);
		data->cmd[i] = ft_strtrim(data->cmd[i], " \t", 1);
		data->step = 1;
		while (still_in(data->cmd[i]))
		{
			dprintf(2, "execution/still in\n");
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
			close(data->fd.redir_fd[1]);
			continue;
		}
		while (still_out(data->cmd[i]))
		{
			dprintf(2, "execution/still out\n");
			data->cmd[i] = redir_out(data, data->cmd[i]);
			if (!data->cmd[i] || !*data->cmd[i])
				break;
		}
		if (!data->cmd[i] || !*data->cmd[i] || data->fd.redir_fd[1] == -1)
		{
			close(data->fd.redir_fd[0]);
			continue;
		}
		if (ft_strcmp(data->cmd[i], "\t") == 0)
		{
			close(data->fd.redir_fd[0]);
			close(data->fd.redir_fd[1]);
			return ;
		}
		data->cmd[i] = ft_strtrim(data->cmd[i], " \t", 1);
		if (i == 0 || (i > 0 && !ft_strcmp(data->ope[i - 1], "|")))
			exec_pipe(data->cmd[i], data);
		else if ((i > 0 && !ft_strcmp(data->ope[i - 1], "||")))
			exec_or(data->cmd[i], data);
		else if ((i > 0 && !ft_strcmp(data->ope[i - 1], "&&")))
			exec_and(data->cmd[i], data);
		if (!ft_strcmp(data->ope[i], "||") || !ft_strcmp(data->ope[i], "&&"))
		{
		 	print(data);
			dup2(data->fd.base_fd[0], 0);
			dup2(data->fd.base_fd[1], 1);
		}
		else if (!ft_strcmp(data->ope[i], "|"))
			if (!isatty(1))
				print(data);
	}
	close(data->fd.redir_fd[0]);
}
