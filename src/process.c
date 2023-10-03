/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 23:23:17 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/22 13:14:31 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	next_it(t_data *data)
{
	if (!ft_strcmp(data->input, ":"))
	{
		g_last_ret = 0;
		return (1);
	}
	if (!ft_strcmp(data->input, ""))
		return (1);
	return (0);
}

static int	get_input(t_data *data)
{
	init_loop(data);
	signals(1);
	if (data->argc == 1)
		data->input = readline(data->prompt);
	if (data->input == NULL)
	{
		printf("exit\n");
		end(data);
	}
	data->input = ft_strtrim(data->input, " \t", 1);
	if (!data->input)
		exit(666);
	if (next_it(data))
	{
		end_loop(data);
		return (1);
	}
	if (history(data))
		return (1);
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
		execution(data);
		if (data->last_pid > 0)
		{
			waitpid(data->last_pid, &status, 0);
			if (g_last_ret != 130 && g_last_ret != 131)
				g_last_ret = WEXITSTATUS(status);
		}
		if (data->active_ret != -1)
			g_last_ret = data->active_ret;
		while (wait(NULL) > 0)
			;
		if (data->argc > 1)
		{
			step0(data);
			exit(g_last_ret);
		}
		end_loop(data);
	}
	return (0);
}
