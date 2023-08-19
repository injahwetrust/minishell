/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 09:53:29 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/18 14:31:12 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	no_newline(char *str)
{
	int	i;

	i = 1;
	if (!str || str[0] != '-')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	echo(char **s_cmd)
{
	int	no_nl;
	int	i;

	i = 0;
	no_nl = 0;
	while (no_newline(s_cmd[++i]))
		no_nl = 1;
	while (s_cmd[i])
	{
		ft_printf("%s", s_cmd[i]);
		if (!s_cmd[i + 1])
			break ;
		write(1, " ", 1);
		i++;
	}
	if (!no_nl)
		write(1, "\n", 1);
	return (0);
}
