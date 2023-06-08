/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:12:41 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/08 22:14:22 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void echo(t_data *data, char *cmd)
{
	int	i;
	int	nl;

	i = 3;
	nl = 1;
	while (cmd[++i])
	{
		if (cmd[i] == '-' && cmd[i + 1] == 'n' && (cmd[i + 2] == ' ' || cmd[i + 2] == '\t'))
			nl = 0;
		if (cmd[i] != ' ' && cmd[i] != '\t')
			break;
	}
	i = 3;
	if (nl == 0)
		while (cmd[i] != 'n')
			i++;
	else
		i = 3;
	i++;
	while (cmd[i] == ' ' || cmd[i] == '\t')
		i++;
	while (cmd[i])
	{
		ft_printf("%c", cmd[i]);
		i++;
	}
	if (nl == 1)
		ft_printf("\n");
	end_process(data);
}

void	recoded(t_data *data, char *cmd)
{
	if (ft_strcmp("pwd", cmd) == 0)
	{
		ft_printf("%s\n", data->cwd);
		end_process(data);
	}
	else if (ft_strcmp("cd", cmd) == 0)
	{
		cmd = ft_strtrim(cmd + 2, " \t", 1);
		chdir(cmd);
		end_process(data);
	}
	else if (ft_strcmp("env", cmd) == 0)
	{
		while (*data->env)
		{
			ft_printf("%s\n", *data->env);
			data->env++;
		}
		end_process(data);
	}
	else if (ft_strcmp("exit", cmd) == 0)
		end_process(data);
	else if (ft_strncmp("echo", cmd, 4) == 0)
		echo(data, cmd);
}
