/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_op_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 00:05:43 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/20 13:32:39 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	fill_op(t_data *data)
{
	int		i;
	int		j;
	char	*clone;

	j = 0;
	clone = data->input;
	while (*clone)
	{
		i = 0;
		edit_lit(data, clone[i]);
		edit_par(data, clone[i]);
		if (in_charset(clone[i], "|&") && !is_lit(data) && !data->par)
		{
			while (in_charset(clone[i], "|&"))
				i++;
			data->cmds[j].next_op = ft_strndup(clone, i, 0);
			j++;
			clone += i;
		}
		else
			clone++;
	}
}

void	fill_cmd(t_data *data)
{
	char	*clone;
	int		i;
	int		j;

	i = 0;
	j = 0;
	clone = data->input;
	while (clone[i])
	{
		(edit_lit(data, clone[i]), edit_par(data, clone[i]));
		if (in_charset(clone[i], "&|") && !is_lit(data) && !data->par)
		{
			data->cmds[j].cmd = ft_strndup(clone, i, 0);
			while (in_charset(clone[i], "&|"))
				i++;
			j++;
			clone += i;
			i = 0;
		}
		else
			i++;
	}
	data->cmds[j].cmd = ft_strndup(clone, i, 0);
	if (!data->cmds[j].cmd)
		exit(666);
}

void	manage_last_cmd(t_data *data)
{
	int		i;
	int		exc;
	char	*new;

	new = ft_strdup("");
	i = -1;
	exc = 0;
	while (data->input[++i])
	{
		if (data->input[i] == '!')
			exc++;
		else
			exc = 0;
		if (exc != 2)
			new = ft_strjoin(new, ft_strndup(data->input + i, 1, 0), 3);
		else
		{
			new = ft_strjoin(ft_strndup(new, ft_strlen(new) - 1, 1),
					data->last_cmd, 1);
			exc = 0;
			data->print = 1;
		}
	}
	free(data->input);
	data->input = new;
}
