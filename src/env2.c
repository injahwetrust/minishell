/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:46:05 by mablatie          #+#    #+#             */
/*   Updated: 2023/09/20 13:33:05 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

int	add_in_env(t_data *data, char *str)
{
	int	i;

	if (parse_export(data, str))
		return (1);
	if (replace_in_env(data, str) || join_in_env(data, str))
		return (0);
	i = 0;
	while (data->env[i])
		i++;
	data->new_env = malloc(sizeof(char *) * (i + 2));
	i = -1;
	while (data->env[++i])
	{
		data->new_env[i] = data->env[i];
	}
	data->new_env[i] = ft_strdup(str);
	data->new_env[i + 1] = 0;
	free(data->env);
	data->env = data->new_env;
	check_ghost(data, str);
	return (0);
}

void	remove_from_ghost(t_data *data, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (data->ghost[i])
		i++;
	data->new_env = malloc(sizeof(char *) * i);
	if (!data->new_env)
		end(data);
	i = 0;
	while (data->ghost[i])
	{
		if (!ft_strncmp(str, data->ghost[i], ft_strlen(str)))
			free (data->ghost[i++]);
		else
			data->new_env[j++] = data->ghost[i++];
	}
	data->new_env[j] = 0;
	free(data->ghost);
	data->ghost = data->new_env;
}

void	remove_from_env_norm(t_data *data, int i)
{
	data->new_env = malloc(sizeof(char *) * i);
	if (!data->new_env)
		exit(666);
}

void	remove_from_env_norm_2(t_data *data, int j)
{
	data->new_env[j] = 0;
	free(data->env);
	data->env = data->new_env;
}

int	remove_from_env(t_data *data, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (is_in_env(data, str) == 1)
	{
		while (data->env[i])
			i++;
		remove_from_env_norm(data, i);
		i = 0;
		while (data->env[i])
		{
			if (!ft_strncmp(str, data->env[i], ft_strlen(str))
				&& data->env[i][ft_strlen(str)] == '=')
				free (data->env[i++]);
			else
				data->new_env[j++] = data->env[i++];
		}
		remove_from_env_norm_2(data, j);
	}
	else if (is_in_env(data, str) == 2)
		remove_from_ghost(data, str);
	return (0);
}
