/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 17:08:34 by mablatie          #+#    #+#             */
/*   Updated: 2023/09/15 17:27:57 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	cancel_cmd_norm(char **s_cmd)
{
	if (ft_strcmp(s_cmd[0], "\t") == 0)
	{
		free (s_cmd[0]);
		s_cmd[0] = ft_strdup("");
	}
}

void	dir_null(t_data *data, DIR *dir, char **s_cmd)
{
	if (dir != NULL)
	{
		closedir(dir);
		ft_dprintf(2, "Minishell: %s: Is a directory\n", s_cmd[0]);
		if (data->paths)
			ft_free_tab(data->paths);
		step0(data);
		exit(126);
	}
}

void	close_and_dup_child(t_data *data)
{
	if (close(data->fd.p_fd[0]) == -1)
		(perror("Minishell"), end(data));
	if (dup2(data->fd.p_fd[1], 1) == -1)
		(perror("Minishell"), end(data));
	if (close(data->fd.p_fd[1]) == -1)
		(perror("Minishell"), end(data));
	if (close(data->fd.base_fd[0]) == -1)
		(perror("Minishell"), end (data));
	if (close(data->fd.base_fd[1]) == -1)
		(perror("Minishell"), end(data));
}

void	exec_norm(t_data *data, int i)
{
	if (ft_strcmp(data->cmds[i].cmd, "./minishell") == 0)
		signals(3);
	data->active_ret = -1;
}
