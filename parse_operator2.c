/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 13:33:31 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/08 13:33:54 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mixed_op(char *cmd, char c)
{
	int	i;

	i = 1;
	while (cmd[i])
	{
		if (cmd[i] == ' ')
			i++;
		else if (in_charset(cmd[i], "|&") && cmd[i] != c && cmd[i + 1] == cmd[i])
			return (dprintf(2, "Minishell: Syntax error near unexpected symbol « %c%c »\n", cmd[i], cmd[i]));
		else if (in_charset(cmd[i], "|&") && cmd[i] != c)
			return (dprintf(2, "Minishell: Syntax error near unexpected symbol « %c »\n", cmd[i]));
		else if (in_charset(cmd[i], "|&") && i > 1 && cmd[i + 1] == cmd[i])
			return (dprintf(2, "Minishell: Syntax error near unexpected symbol « %c%c »\n", cmd[i], cmd[i]));
		else if (in_charset(cmd[i], "|&") && i > 1)
			return (dprintf(2, "Minishell: Syntax error near unexpected symbol « %c »\n", cmd[i]));
		else
			return (0);
	}
	return (0);
}
