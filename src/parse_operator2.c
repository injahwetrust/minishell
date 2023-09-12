/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 13:33:31 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/24 17:12:56 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	mixed_op2(char *cmd, char c, int i)
{
	int	n;

	n = 0;
	if (c == '<' && cmd[i] == '>')
	{
		while (cmd[i] && cmd[i++] == '>')
			n++;
		if (n == 2)
			return (ft_dprintf(2, BRACK_ERR_1));
		if (n >= 3)
			return (ft_dprintf(2, BRACK_ERR_2));
	}
	else if (c == '>' && cmd[i] == '<')
	{
		while (cmd[i] && cmd[i++] == '<')
			n++;
		if (n == 1)
			return (ft_dprintf(2, RBRACK_ERR_1));
		if (n == 2)
			return (ft_dprintf(2, RBRACK_ERR_2));
		if (n >= 3)
			return (ft_dprintf(2, RBRACK_ERR_3));
	}
	return (0);
}

int	mixed_op(char *cmd, char c)
{
	int	i;

	i = 1;
	while (cmd[i])
	{
		if (cmd[i] == ' ')
			i++;
		else if (mixed_op2(cmd, c, i))
			return (1);
		else if (in_charset(cmd[i], "|&")
			&& cmd[i] != c && cmd[i + 1] == cmd[i])
			return (ft_dprintf(2, MINI PARSE_ERR_SYCH_2, cmd[i], cmd[i]));
		else if (in_charset(cmd[i], "|&") && cmd[i] != c)
			return (ft_dprintf(2, PARSE_ERR_SYCH, cmd[i]));
		else if (in_charset(cmd[i], "|&") && i > 1 && cmd[i + 1] == cmd[i])
			return (ft_dprintf(2, MINI PARSE_ERR_SYCH_2, cmd[i], cmd[i]));
		else if (in_charset(cmd[i], "|&") && i > 1)
			return (ft_dprintf(2, PARSE_ERR_SYCH, cmd[i]));
		else
			return (0);
	}
	return (0);
}

static int	count_space(char *cmd, char c)
{
	int	i;
	int	n;

	i = -1;
	n = 0;
	while (cmd[++i])
	{
		if (cmd[i] == c && i > 1)
		{
			while (cmd[i++] == c)
				n++;
			if (n == 1 || n == 2)
				return (ft_dprintf(2, PARSE_ERR_SYCH, c));
			if (n >= 3 && c == '<')
				return (ft_dprintf(2, MINI COUNT_S PARSE_ERR_SYCH_3, c, c, c));
			if (n >= 3)
				return (ft_dprintf(2, MINI PARSE_ERR_SYCH_2, c, c));
		}
		if (cmd[i] != c && cmd[i] != ' ')
			return (0);
	}
	return (0);
}

int	false_space(t_data *data)
{
	int	i;

	i = 0;
	while (data->input[i])
	{
		if (data->input[i] == '<' || data->input[i] == '>')
			if (count_space(data->input + i, data->input[i]))
				return (1);
		i++;
	}
	return (0);
}
