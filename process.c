/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 23:23:17 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/16 16:33:03 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_input(t_data *data)
{
	init_loop(data);
	signals(1);
	if (data->argc == 1)
		data->input = readline(data->prompt);
	if (data->input == NULL)
		end(data);
	data->input = ft_strtrim(data->input, " \t", 1);
	if (!ft_strcmp(data->input, ""))
	{
		end_loop(data);
		return (1);
	}
	add_history(data->input);
	if (parse_input(data))
	{
		end_loop(data);
		return (1);
	}
	return (0);
}

int	process(t_data *data)
{
	int	status;
	while (1)
	{
		if (get_input(data))
			continue ;
		if (ft_strcmp(data->input, "exit") == 0)
		{
			end(data);
			printf("exit\n");
		}
		execution(data);
		if (data->last_pid > 0)
		{
			waitpid(data->last_pid, &status, 0);
			data->last_ret = WEXITSTATUS(status);
		}
		while (wait(NULL) > 0)
			;
		printf("data->last_pid = %d\ndata->last_ret = %d\n", data->last_pid, data->last_ret);
		if (data->argc > 1)
			end(data);
		end_loop(data);
	}
	return (0);
}
