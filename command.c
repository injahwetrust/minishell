/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 22:28:42 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/26 14:38:21 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	canceled_built_in(t_data *data, char **s_cmd)
{
	int	ret;

	ret = -1;
	if (!ft_strcmp("cd", s_cmd[0]))
		ret = cd(data, s_cmd);
	else if (!ft_strcmp("export", s_cmd[0]))
		ret = add_in_env(data, s_cmd[1]);
	else if (!ft_strcmp("unset", s_cmd[0]))
		ret = 0;
	else if (!ft_strcmp("exit", s_cmd[0]) && !s_cmd[1])
		ret = 0;
	else if (!ft_strcmp("exit", s_cmd[0]) && s_cmd[1])
		ret = ft_atoi(s_cmd[1]);
	if (ret != -1)
	{
		step0(data);
		exit(ret);
	}
}

void	constant_built_in(t_data *data, char **s_cmd)
{
	int	ret;

	ret = -1;
	if (ft_strcmp("echo", s_cmd[0]) == 0)
		ret = echo(s_cmd);
	else if (ft_strcmp("env", s_cmd[0]) == 0)
		ret = env(data, s_cmd);
	else if (ft_strcmp("pwd", s_cmd[0]) == 0)
	{
		printf("%s\n", data->cwd);
		ret = 0;
	}
	else if (ft_strcmp("export", s_cmd[0]) == 0 && !s_cmd[1])
		ret = print_declare(data);
	if (ret != -1)
	{
		step0(data);
		exit(ret);
	}
}

int	active_built_in(t_data *data, char **s_cmd)
{
	int	ret;
	int	i;

	i = 2;
	ret = -1;
	if (ft_strcmp("cd", s_cmd[0]) == 0)
		ret = cd(data, s_cmd);
	else if (ft_strcmp("export", s_cmd[0]) == 0 && s_cmd[1])
	{
		ret = add_in_env(data, s_cmd[1]);
		while (s_cmd[i] && !ret)
		{
			ret = add_in_env(data, s_cmd[i]);
			i++;
		}
	}
	else if (ft_strcmp("unset", s_cmd[0]) == 0)
		ret = remove_from_env(data, s_cmd[1]);
	else if (ft_strcmp("exit", s_cmd[0]) == 0 && s_cmd[1])
		ret = manage_exit(data, s_cmd);
	else if (ft_strcmp("exit", s_cmd[0]) == 0 && !s_cmd[1])
	{
		printf("exit\n");
		end(data);
	}
	return (ret);
}
