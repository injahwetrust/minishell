/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 22:28:42 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/18 14:31:20 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

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

	ret = -1;
	if (ft_strcmp("cd", s_cmd[0]) == 0)
		ret = cd(data, s_cmd);
	else if (ft_strcmp("export", s_cmd[0]) == 0 && s_cmd[1])
	{
		ret = add_in_env(data, s_cmd[1]);
		i = 1;
		while (s_cmd[++i] && !ret)
			ret = add_in_env(data, s_cmd[i]);
	}
	else if (ft_strcmp("unset", s_cmd[0]) == 0)
	{
		i = 0;
		while (s_cmd[++i])
			ret = remove_from_env(data, s_cmd[i]);
	}
	else if (ft_strcmp("exit", s_cmd[0]) == 0 && s_cmd[1])
		ret = manage_exit(data, s_cmd);
	else if (ft_strcmp("exit", s_cmd[0]) == 0 && !s_cmd[1])
	{
		printf("exit\n");
		end(data);
	}
	return (ret);
}