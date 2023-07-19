/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:21:34 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/19 13:47:19 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	too_many_arg(t_data *data, char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (!ft_isdigit(cmd[i]))
		{
			printf("exit\n");
			dprintf(2, "bash: exit: %s: numeric argument required\n", cmd);
			step0(data);
			exit(2);
		}
		i++;
	}
	printf("exit\n");
	dprintf(2, "Minishell: exit: too many arguments\n");
	return (1);
}

static void	parse_exit(t_data *data, char *cmd)
{
	int	i;
	unsigned long long	ret;

	i = -1;
	while (cmd[++i])
	{
		if (i == 0 && (cmd[i] == '-' || cmd[i] == '+'))
			continue ;
		if (!ft_isdigit(cmd[i]))
		{
			printf("exit\n");
			dprintf(2, "bash: exit: %s: numeric argument required\n", cmd);
			step0(data);
			exit(2);
		}
	}
	ret = ft_atoll(cmd);
	step0(data);
	exit (ret);
}

int	manage_exit(t_data *data, char **s_cmd)
{
	int	i;

	i = 0;
	while (s_cmd[i])
		i++;
	if (i > 2)
		return (too_many_arg(data, s_cmd[1]));
	else
		parse_exit(data, s_cmd[1]);
	return (0);
}