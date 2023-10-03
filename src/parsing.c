/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 00:02:28 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/22 14:00:01 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

int	parse_export(t_data *data, char *input)
{
	int	i;

	i = 0;
	if (in_charset(input[0], "0123456789="))
	{
		ft_dprintf(2, MINI " export: « %s » not a valid identifier\n", input);
		return (1);
	}
	while (input[i] && input[i] != '=')
	{
		if (i > 0 && input[i] == '+' && input[i + 1] == '=')
			break ;
		if (!in_charset(input[i], data->ex))
		{
			ft_dprintf(2, EXPORT_ERR, input);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	first_char(t_data *data)
{
	int		i;
	char	c;

	i = 0;
	c = 0;
	if (in_charset(data->input[0], "&|;"))
	{
		c = data->input[0];
		while (data->input[i] == c)
			i++;
		if (i == 1)
			return (ft_dprintf(2, PARSE_ERR_SYCH, c));
		if (i >= 2)
			return (ft_dprintf(2, MINI PARSE_ERR_SYCH_2, c, c));
	}
	return (0);
}

static int	empty_par(t_data *data)
{
	int	i;
	int	j;

	i = -1;
	while (data->input[++i])
	{
		if (data->input[i] == '(')
		{
			j = i + 1;
			while (data->input[j] && data->input[j] == ' ')
				j++;
			if (data->input[j++] == ')')
				return (ft_dprintf(2, CPAR_ERR));
		}
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
		g_last_ret = 1;
		return (1);
	}
	if (parse_cmd(data) || parse_op(data))
	{
		g_last_ret = 2;
		return (1);
	}
	manage_last_cmd(data);
	if (!data->last_cmd)
		exit(666);
	if (data->print)
		printf("%s\n", data->input);
	data->heredoc_ret = g_last_ret;
	manage_dollar(data);
	g_last_ret = -1;
	data->input = wildcards(data);
	stock(data);
	bash_morphing(data);
	manage_lit(data);
	return (0);
}
