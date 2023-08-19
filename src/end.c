/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 23:59:30 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/18 14:59:58 by mablatie         ###   ########.fr       */
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
	unlink("/tmp/free_gnl");
	close(data->fd.base_fd[0]);
	close(data->fd.base_fd[1]);
	free(data->input);
	free(data->prompt);
	free(data->cwd);
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
	free(data->cwd);
	if (data->step > 0)
	{
		free_cmds(data);
	}
}
