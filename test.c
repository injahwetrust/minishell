/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 23:41:35 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/17 02:16:18 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	info(t_data *data)
{
	int	i;
	int	j;
	
	i = 0;
	dprintf(2, "----------------------\n");
	dprintf(2, "data->input = °%s°\n", data->input);
	while (i < data->count)
	{
		j = 0;
		dprintf(2, "cmd °%d°\n", i);
		dprintf(2, "cmd = °%s°\n", data->cmds[i].cmd);
		dprintf(2, "prev_op = °%s°\n", data->cmds[i].prev_op);
		dprintf(2, "next_op = °%s°\n", data->cmds[i].next_op);
		while (data->cmds[i].in[j])
		{
			dprintf(2, "in %d = °%s°\n", j, data->cmds[i].in[j]);
			j++;
		}
		j = 0;
		while (data->cmds[i].out[j])
		{
			dprintf(2, "out %d = °%s°\n", j, data->cmds[i].out[j]);
			j++;
		}
		j = 0;
		while (data->cmds[i].s_cmd[j])
		{
			dprintf(2, "s_cmd[%d] = °%s°\n", j, data->cmds[i].s_cmd[j]);
			j++;
		}
		dprintf(2, "----------------------\n");
		i++;
	}
	dprintf(2, "data->last_pid = %d\ndata->last_ret = %d\n", data->last_pid, data->last_ret);
	return (0);
}
