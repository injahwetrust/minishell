/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:03:35 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/15 17:27:44 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	add_in_ghost(t_data *data, char *str)
{
	char	**new_ghost;

	if (is_in_env(data, str))
		return ;
	new_ghost = ft_tabdup(data->ghost, str, 1);
	data->ghost = new_ghost;
}

int	replace_in_env(t_data *data, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
	{
		add_in_ghost(data, str);
		return (1);
	}
	while (data->env[j])
	{
		if (ft_strncmp(data->env[j], str, i) == 0)
		{
			free(data->env[j]);
			data->env[j] = ft_strdup(str);
			return (1);
		}
		j++;
	}
	return (0);
}

int	join_in_env(t_data *data, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '+')
		i++;
	if (!str[i] || str[i + 1] != '=')
		return (0);
	while (data->env[j])
	{
		if (ft_strncmp(data->env[j], str, i) == 0)
		{
			data->env[j] = ft_strjoin(data->env[j], str + i + 2, 1);
			return (1);
		}
		j++;
	}
	return (0);
}

void	print_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		printf("%s\n", data->env[i]);
		i++;
	}
}

int	env(t_data *data, char **s_cmd)
{
	int	i;

	i = 0;
	while (s_cmd[i])
	{
		if (ft_strcmp(s_cmd[i], "env") != 0)
		{
			errno = ENOENT;
			ft_dprintf(2, "Minishell: «%s»: %s\n", s_cmd[i], strerror(errno));
			return (127);
		}
		i++;
	}
	print_env(data);
	return (0);
}
