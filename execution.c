/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 16:41:30 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/26 14:57:22 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	go(t_data *data, char **s_cmd)
{
	char	*path;
	DIR	*dir;

	edit_paths(data, s_cmd[0]);
	dir = opendir(s_cmd[0]);
	if (dir != NULL)
	{
		closedir(dir);
		ft_dprintf(2, "Minishell: %s: Is a directory\n", s_cmd[0]);
		if (data->paths)
			ft_free_tab(data->paths);
		step0(data);
		exit(126);
	}
	path = get_exec(s_cmd[0], data);
	if (execve(path, s_cmd, data->env) <= -1)
	{
		dprintf(2, "Minishell: %s : command not found\n", s_cmd[0]);
		if (data->paths)
			ft_free_tab(data->paths);
		step0(data);
		exit(127);
	}
}

void	exec_pipe(t_data *data, char **s_cmd)
{
	int	pid;

	if (pipe(data->fd.p_fd) == -1)
		end(data);
	pid = fork();
	if (pid == 0)
	{
		close(data->fd.p_fd[0]);
		dup2(data->fd.p_fd[1], 1);
		close(data->fd.p_fd[1]);
		close(data->fd.base_fd[0]);
		close(data->fd.base_fd[1]);
		constant_built_in(data, s_cmd);
		canceled_built_in(data, s_cmd);
		go(data, s_cmd);
		exit(0);
	}
	data->last_pid = pid;
	close(data->fd.p_fd[1]);
	dup2(data->fd.p_fd[0], 0);
	close(data->fd.p_fd[0]);
}

void	simple_exec(t_data *data, char **s_cmd)
{
	int	pid;
	int	ret;

	ret = active_built_in(data, s_cmd);
	if (ret != -1)
	{
		last_ret = ret;
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		close(data->fd.base_fd[0]);
		close(data->fd.base_fd[1]);
		constant_built_in(data, s_cmd);
		go(data, s_cmd);
		exit(0);
	}
	data->last_pid = pid;
}

int	cancel_cmd(t_data *data, char *op, char **s_cmd)
{
	int	status;

	if (ft_strcmp(s_cmd[0], "") == 0)
		return (last_ret = 0, 1);
	if (ft_strcmp(s_cmd[0], "\t") == 0)
	{
		free (s_cmd[0]);
		s_cmd[0] = ft_strdup("");
	}
	if (ft_strcmp(op, "&&") == 0)
	{
		waitpid(data->last_pid, &status, 0);
		status %= 255;
		if (status > 0)
			return (1);
	}
	else if (ft_strcmp(op, "||") == 0)
	{
		waitpid(data->last_pid, &status, 0);
		status %= 255;
		if (status == 0)
			return (1);
	}
	return (0);
}

void	execution(t_data *data)
{
	int	i;
	
	i = -1;
	creation(data);
	signals(2);
	while (++i < data->count)
	{
		if (redirection(data, &data->cmds[i]))
			continue ;
		if (cancel_cmd(data, data->cmds[i].prev_op, data->cmds[i].s_cmd))
			continue ;
		if (ft_strcmp(data->cmds[i].prev_op, "|") == 0 || ft_strcmp(data->cmds[i].next_op, "|") == 0)
			exec_pipe(data, data->cmds[i].s_cmd);
		else
			simple_exec(data, data->cmds[i].s_cmd);
		if (!isatty(0) && !isatty(1))
			print();
		else if (!isatty(0) && i == data->count - 1)
			print();
	}
}
