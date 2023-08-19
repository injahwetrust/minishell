/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:03:35 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/18 15:04:13 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	add_in_ghost(t_data *data, char *str)
{
	char	**new_ghost;

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
		if (!strncmp(str, data->ghost[i], ft_strlen(str)))
			free (data->ghost[i++]);
		else
			data->new_env[j++] = data->ghost[i++];
	}
	data->new_env[j] = 0;
	free(data->ghost);
	data->ghost = data->new_env;
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
		data->new_env = malloc(sizeof(char *) * i);
		if (!data->new_env)
			end(data);
		i = 0;
		while (data->env[i])
		{
			if (!strncmp(str, data->env[i], ft_strlen(str)) && data->env[i][ft_strlen(str)] == '=')
				free (data->env[i++]);
			else
				data->new_env[j++] = data->env[i++];
		}
		data->new_env[j] = 0;
		free(data->env);
		data->env = data->new_env;
	}
	else if (is_in_env(data, str) == 2)
		remove_from_ghost(data, str);
	return (0);
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
			dprintf(2, "Minishell: «%s»: %s\n", s_cmd[i], strerror(errno));
			return (127);
		}
		i++;
	}
	print_env(data);
	return (0);
}