/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 23:59:30 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/12 14:15:34 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static void	free_cmds(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		free(data->cmds[i].cmd);
		ft_free_tab(data->cmds[i].in);
		ft_free_tab(data->cmds[i].out);
		ft_free_tab(data->cmds[i].s_cmd);
		if (i < data->count - 1)
			free (data->cmds[i].next_op);
		i++;
	}
	free(data->cmds);
}

void	step0(t_data *data)
{
	rl_clear_history();
	close(data->fd.base_fd[0]);
	close(data->fd.base_fd[1]);
	free(data->input);
	free(data->prompt);
	free(data->last_cmd);
	ft_free_tab(data->env);
	ft_free_tab(data->ghost);
	if (data->step > 0)
		free_cmds(data);
}

void	end(t_data *data)
{
	step0(data);
	exit(0);
}

void	end_loop(t_data *data)
{
	free(data->input);
	free(data->prompt);
	unlink("/tmp/mini_here_doc");
	if (data->step > 0)
	{
		free_cmds(data);
	}
}

void	step1(t_data *data)
{
	free(data->last_cmd);
	free(data->ghost);
	close(data->fd.base_fd[0]);
	close(data->fd.base_fd[1]);
	exit(0);
}
