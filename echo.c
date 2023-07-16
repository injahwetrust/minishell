/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 09:53:29 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/14 14:44:21 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	newline(char *str)
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
	int	nl;
	int	i;

	nl = newline(s_cmd[1]);
	i = 1 + nl;
	while (s_cmd[i])
	{
		ft_printf("%s", s_cmd[i]);
		if (!s_cmd[i + 1])
			break ;
		write(1, " ", 1);
		i++;
	}
	if (!nl)
		write(1, "\n", 1);
	return (0);
}
