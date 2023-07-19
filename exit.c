/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 12:21:34 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/19 16:44:26 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void checkflow(t_data *data, char *cmd, char *max, int i)
{
	long long	ret;

	max[0] -= 1;
	cmd[i] -= 1;
	ret = ft_atoll(cmd);
	if (ret >= ft_atoll(max))
	{
		free(max);
		printf("exit\n");
		dprintf(2, "Minishell: exit: %s: numeric argument required\n", cmd);
		step0(data);
		exit(2);
	}
	else
	{
		free(max);
		printf("exit\n");
		step0(data);
		exit(ret);
	}
}

static void	overflow(t_data *data, char *cmd, char *max)
{
		free(max);
		printf("exit\n");
		dprintf(2, "bash: exit: %s: numeric argument required\n", cmd);
		step0(data);
		exit(2);
}

static void underflow(t_data *data, char *cmd, char *max)
{
	long long ret;
		
	free(max);
	ret = ft_atoll(cmd);
	step0(data);
	printf("exit\n");
	exit(ret);
}

static void	check_overflow(t_data *data, char *cmd)
{
	char	*max;
	int		size_cmd;
	int		size_max;
	int		i;

	i = -1;
	max = ft_strdup("9223372036854775808");
	size_cmd = ft_strlen(cmd);
	size_max = ft_strlen(max);
	if (cmd[0] == '-' || cmd[0] == '+')
	{
		i++;
		size_cmd--;
	}
	while (cmd[++i] == '0')
		size_cmd -= 1;
	if (size_cmd < size_max)
		underflow(data, cmd, max);
	else if (size_cmd > size_max)
		overflow(data, cmd, max);
	else
		checkflow(data, cmd, max, i);
}

static void	parse_exit(t_data *data, char *cmd)
{
	int	i;

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
	check_overflow(data, cmd);
}

static int	too_many_arg(t_data *data, char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (!ft_isdigit(cmd[i]))
		{
			printf("exit\n");
			dprintf(2, "Minishell: exit: %s: numeric argument required\n", cmd);
			step0(data);
			exit(2);
		}
		i++;
	}
	printf("exit\n");
	dprintf(2, "Minishell: exit: too many arguments\n");
	return (1);
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