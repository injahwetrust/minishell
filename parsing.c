/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 00:02:28 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/05 20:56:05 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_export(t_data *data, char *input)
{
	int	i;

	i = 0;
	if (in_charset(input[0], "0123456789="))
	{
		dprintf(2, "Minishell: export: « %s » not a valid identifier\n", input);
		return (1);
	}
	while (input[i] && input[i] != '=')
	{
		if (i > 0 && input[i] == '+' && input[i + 1] == '=')
			break ;
		if (!in_charset(input[i], data->ex))
		{
				dprintf(2, "Minishell: export: « %s » not a valid identifier\n", input);
				return (1);
		}
		i++;
	}
	return (0);
}

static int	first_char(t_data *data)
{
	int	i;
	char c;

	i = 0;
	c = 0;
	if (in_charset(data->input[0], "&|;"))
	{
		c = data->input[0];
		while (data->input[i] == c)
			i++;
		if (i == 1)
			return (dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", c));
		if (i >= 2)
			return (dprintf(2, "Minishell: Syntax error near unexpected token « %c%c »\n", c, c));
	}
	return (0);
}

static int	empty_par(t_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (data->input[++i])
		if (data->input[i] == '(')
		{
			j = i + 1;
			while (data->input[j] && data->input[j] == ' ')
				j++;
			if (data->input[j++] == ')')
				return (dprintf(2, "Minishell: Syntax error near unexpected token « ) »\n"));
		}
	return (0);
}

static int	parse_cmd(t_data *data)
{
	if (first_char(data))
		return (1);
	if (empty_par(data))
		return (1);
	return (0);
}

int	parse_input(t_data *data)
{
	if (remove_tab(data, data->input))
		return (1);
	if (ft_strcmp(data->input, "!") == 0)
	{
		last_ret = 1;
		return (1);
	}
	if (parse_cmd(data) || parse_op(data))
	{
		last_ret = 2;
		return (1);
	}
	manage_last_cmd(data);
	free(data->last_cmd);
	data->last_cmd = ft_strdup(data->input);
	if (data->print)
		printf("%s\n", data->input);
	manage_dollar(data);
	last_ret = -1;
	data->input = wildcards(data);
	stock(data);
	bash_morphing(data);
	manage_lit(data);
	return (0);
}
