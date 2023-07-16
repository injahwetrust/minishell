/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 23:41:35 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/12 12:38:29 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	info(t_data *data)
{
	int	i;
	int	j;
	
	i = 0;
	printf("----------------------\n");
	printf("data->input = °%s°\n", data->input);
	while (i < data->count)
	{
		j = 0;
		printf("cmd °%d°\n", i);
		printf("cmd = °%s°\n", data->cmds[i].cmd);
		printf("prev_op = °%s°\n", data->cmds[i].prev_op);
		printf("next_op = °%s°\n", data->cmds[i].next_op);
		while (data->cmds[i].in[j])
		{
			printf("in %d = °%s°\n", j, data->cmds[i].in[j]);
			j++;
		}
		j = 0;
		while (data->cmds[i].out[j])
		{
			printf("out %d = °%s°\n", j, data->cmds[i].out[j]);
			j++;
		}
		j = 0;
		while (data->cmds[i].s_cmd[j])
		{
			printf("s_cmd[%d] = °%s°\n", j, data->cmds[i].s_cmd[j]);
			j++;
		}
		printf("----------------------\n");
		i++;
	}
	return (0);
}
