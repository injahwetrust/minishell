/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 22:28:42 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/17 01:20:15 by bvaujour         ###   ########.fr       */
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
	else if (ft_strcmp("history", s_cmd[0]) == 0)
		ret = print_history();
	else if (ft_strcmp("save", s_cmd[0]) == 0)
		ret = save_cmd(data, s_cmd);
	else if (ft_strcmp("save", s_cmd[0]) == 0)
		ret = save_cmd(data, s_cmd);
	if (ret != -1)
	{
		step0(data);
		exit(ret);
	}
}

void	active_built_in_norm(t_data *data, int *ret, int i, char **s_cmd)
{
	if (ft_strcmp("cd", s_cmd[0]) == 0)
		*ret = cd(data, s_cmd);
	else if (ft_strcmp("export", s_cmd[0]) == 0 && s_cmd[1])
	{
		*ret = add_in_env(data, s_cmd[1]);
		i = 1;
		while (s_cmd[++i] && !*ret)
			*ret = add_in_env(data, s_cmd[i]);
	}
	else if (ft_strcmp("unset", s_cmd[0]) == 0)
	{
		i = 0;
		while (s_cmd[++i])
			*ret = remove_from_env(data, s_cmd[i]);
	}
	else if (ft_strcmp("exit", s_cmd[0]) == 0 && s_cmd[1])
		*ret = manage_exit(data, s_cmd);
	else if (ft_strcmp("exit", s_cmd[0]) == 0 && !s_cmd[1])
	{
		printf("exit\n");
		end(data);
	}
	else if (ft_strcmp("rmcmd", s_cmd[0]) == 0)
		*ret = delete_cmd(data, s_cmd);
}

int	active_built_in(t_data *data, char **s_cmd)
{
	int	ret;
	int	i;

	ret = -1;
	i = 0;
	active_built_in_norm(data, &ret, i, s_cmd);
	return (ret);
}
