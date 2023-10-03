/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 17:31:00 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/20 13:28:39 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	pass_cmd(t_data *data, int *i)
{
	if (data->input[*i] == '|')
	{
		while (data->input[*i] == '|')
			*i += 1;
		data->count++;
	}
	else if (data->input[*i] == '&')
	{
		while (data->input[*i] == '&')
			*i += 1;
		data->count++;
	}
	else
		*i += 1;
	return (0);
}

static int	count_cmd(t_data *data)
{
	int	i;

	i = 0;
	while (data->input[i])
	{
		edit_lit(data, data->input[i]);
		edit_par(data, data->input[i]);
		if (data->lit || data->d_lit || data->par)
		{
			i++;
			continue ;
		}
		pass_cmd(data, &i);
	}
	return (0);
}

void	trim_cmds(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		data->cmds[i].cmd = ft_strtrim(data->cmds[i].cmd, " ", 1);
		if (!data->cmds[i].cmd)
			exit(666);
		if (i > 0)
			data->cmds[i].prev_op = data->cmds[i - 1].next_op;
		i++;
	}
}

void	stock(t_data *data)
{
	count_cmd(data);
	data->step++;
	data->cmds = malloc(sizeof(t_cmd) * data->count);
	if (!data->cmds)
		exit(666);
	init_cmds(data);
	fill_cmd(data);
	fill_op(data);
	fill_in(data);
	fill_out(data);
	trim_cmds(data);
	fill_s_cmd(data);
}
