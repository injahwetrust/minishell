/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:59:50 by mablatie          #+#    #+#             */
/*   Updated: 2023/08/24 17:00:06 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	underflow(t_data *data, char *cmd, char *max)
{
	long long	ret;

	free(max);
	ret = ft_atoll(cmd);
	step0(data);
	printf("exit\n");
	exit(ret);
}

void	checkflow(t_data *data, char *cmd, char *max, int i)
{
	long long	ret;

	max[0] -= 1;
	cmd[i] -= 1;
	ret = ft_atoll(cmd);
	if ((ret >= 0 && ret >= ft_atoll(max)) || (ret < 0 && ret < -ft_atoll(max)))
	{
		cmd[i] += 1;
		free(max);
		printf("exit\n");
		ft_dprintf(2, "Minishell: exit: %s: numeric argument required\n", cmd);
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
