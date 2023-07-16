/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 00:02:28 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/16 20:55:19 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_export(t_data *data, char *input)
{
	int	i;

	i = 0;
	if (in_charset(input[0], "0123456789"))
	{
		dprintf(2, "Minishell: export: « %s » wrong identifier\n", input);
		return (1);
	}
	while (input[i])
	{
		if (!in_charset(input[i], data->ex))
		{
			dprintf(2, "Minishell: export: « %s » wrong identifier\n", input);
			return (1);
		}
		i++;
	}
	return (0);
}

void	manage_last_cmd(t_data *data)
{
	int	i;
	int	exc;
	char	*new;

	new = ft_strdup("");
	i = 0;
	exc = 0;
	while (data->input[i])
	{
		if (data->input[i] == '!')
			exc++;
		else
			exc = 0;
		if (exc != 2)
			new = ft_strjoin(new, ft_strndup(data->input + i, 1, 0), 3);
		else
		{
			new = ft_strjoin(ft_strndup(new, ft_strlen(new) - 1, 1), data->last_cmd, 1);
			exc = 0;
		}
		printf("new = |%s|\n", new);
		i++;
	}
	free(data->input);
	data->input = new;
}

int	parse_input(t_data *data)
{
	if (remove_tab(data, data->input))
		return (1);
	if (ft_strcmp(data->input, "!") == 0)
	{
		data->last_ret = 1;
		return (1);
	}
	manage_last_cmd(data);
	free(data->last_cmd);
	data->last_cmd = ft_strdup(data->input);
	if (parse_op(data))
		return (1);
	data->input = wildcards(data);
	stock(data);
	manage_dollar(data);
	return (0);
}
