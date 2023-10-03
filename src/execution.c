/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 16:41:30 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/22 17:32:27 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	go(t_data *data, char **s_cmd)
{
	char	*path;
	DIR		*dir;

	edit_paths(data, s_cmd[0]);
	dir = opendir(s_cmd[0]);
	dir_null(data, dir, s_cmd);
	path = get_exec(s_cmd[0], data);
	if (execve(path, s_cmd, data->env) <= -1)
	{
		if (errno == 13)
			ft_dprintf(2, "Minishell: %s: %s\n", s_cmd[0], strerror(errno));
		else
			error_print(s_cmd[0]);
		if (data->paths)
			ft_free_tab(data->paths);
		step0(data);
		if (errno == 13)
			exit(126);
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
		close_and_dup_child(data);
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

	if (ft_strcmp("./minishell", s_cmd[0]) == 0)
		signals(4);
	ret = active_built_in(data, s_cmd);
	if (ret != -1)
	{
		data->active_ret = ret;
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
	if (ft_strcmp(s_cmd[0], "") == 0)
		return (g_last_ret = 0, 1);
	cancel_cmd_norm(s_cmd);
	if (ft_strcmp(op, "&&") == 0)
	{
		if (data->last_pid != -1)
		{
			waitpid(data->last_pid, &g_last_ret, 0);
			g_last_ret %= 255;
		}
		if (g_last_ret > 0)
			return (data->last_pid = -1, 1);
	}
	else if (ft_strcmp(op, "||") == 0)
	{
		if (data->last_pid != -1)
		{
			waitpid(data->last_pid, &g_last_ret, 0);
			g_last_ret %= 255;
		}
		if (g_last_ret == 0)
			return (data->last_pid = -1, 1);
	}
	return (0);
}

void	execution(t_data *data)
{
	int	i;

	i = -1;
	(creation(data), signals(2));
	while (++i < data->count)
	{
		exec_norm(data, i);
		if (redirection(data, &data->cmds[i]) || g_last_ret == 130)
			continue ;
		if (cancel_cmd(data, data->cmds[i].prev_op, data->cmds[i].s_cmd))
			continue ;
		if (ft_strcmp(data->cmds[i].prev_op, "|") == 0
			|| ft_strcmp(data->cmds[i].next_op, "|") == 0)
			exec_pipe(data, data->cmds[i].s_cmd);
		else
			simple_exec(data, data->cmds[i].s_cmd);
		if (!isatty(0) && !isatty(1))
			print();
		else if (!isatty(0) && i == data->count - 1
			&& ft_strcmp(data->cmds[i].prev_op, "|") == 0)
			print();
		dup2(data->fd.base_fd[1], 1);
		if (ft_strcmp(data->cmds[i].next_op, "|") != 0)
			dup2(data->fd.base_fd[0], 0);
	}
}
