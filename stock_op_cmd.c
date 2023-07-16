/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_op_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 00:05:43 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/08 00:14:31 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_op(t_data *data)
{
	int	i;
	int	j;
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
		edit_lit(data, clone[i]);
		edit_par(data, clone[i]);
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
}
