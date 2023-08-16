/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 13:33:31 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/06 00:48:31 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	mixed_op2(char *cmd, char c, int i)
{
	int	n;
	
	n = 0;
	if (c == '<' && cmd[i] == '>')
	{
		while (cmd[i] && cmd[i++] == '>')
			n++;
		if (n == 2)
			return (dprintf(2, "Minishell: Syntax error near unexpected token « > »\n"));
		if (n >= 3)
			return (dprintf(2, "Minishell: Syntax error near unexpected token « >> »\n"));
	}
	else if (c == '>' && cmd[i] == '<')
	{
		while (cmd[i] && cmd[i++] == '<')
			n++;
		if (n == 1)
			return (dprintf(2, "Minishell: Syntax error near unexpected token « < »\n"));
		if (n == 2)
			return (dprintf(2, "Minishell: Syntax error near unexpected token « << »\n"));
		if (n >= 3)
			return (dprintf(2, "Minishell: Syntax error near unexpected token « <<< »\n"));
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
		else if (in_charset(cmd[i], "|&") && cmd[i] != c && cmd[i + 1] == cmd[i])
			return (dprintf(2, "Minishell: Syntax error near unexpected token « %c%c »\n", cmd[i], cmd[i]));
		else if (in_charset(cmd[i], "|&") && cmd[i] != c)
			return (dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", cmd[i]));
		else if (in_charset(cmd[i], "|&") && i > 1 && cmd[i + 1] == cmd[i])
			return (dprintf(2, "Minishell: Syntax error near unexpected token « %c%c »\n", cmd[i], cmd[i]));
		else if (in_charset(cmd[i], "|&") && i > 1)
			return (dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", cmd[i]));
		else
			return (0);
		//dprintf(2, "mixed_op passed\n");
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
				return (dprintf(2, "Minishell count space: Syntax error near unexpected token « %c »\n", c));
			if (n >= 3 && c == '<')
				return (dprintf(2, "Minishell count space: Syntax error near unexpected token « %c%c%c »\n", c, c, c));
			if (n >= 3)
				return (dprintf(2, "Minishell count space: Syntax error near unexpected token « %c%c »\n", c, c));
			
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
