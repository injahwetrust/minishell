/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 10:03:35 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/16 16:25:59 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_in_ghost(t_data *data, char *str)
{
	char	**new_ghost;

	new_ghost = ft_tabdup(data->ghost, str, 1);
	data->ghost = new_ghost;
}

int	replace_in_env(t_data *data, char *str)
{
	int	i;
	int j;
	
	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
	{
		printf("add in ghost\n");
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

int    add_in_env(t_data *data, char *str)
{
    int i;
	
	if (parse_export(data, str))
	{
		printf("export reject %s\n", str);
		return (1);
	}
	if (replace_in_env(data, str))
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

int	remove_from_env(t_data *data, char *str)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	if (!str || !is_in_env(data, str))
		return (0);
	while (data->env[i])
		i++;
	data->new_env = malloc(sizeof(char *) * i);
	if (!data->new_env)
		end(data);
	i = 0;
	while (data->env[i])
	{
		if (!strncmp(str, data->env[i], ft_strlen(str)))
			free (data->env[i++]);
		else
			data->new_env[j++] = data->env[i++];
	}
	data->new_env[j] = 0;
	free(data->env);
	data->env = data->new_env;
	return (0);
}

int	env(t_data *data, char **s_cmd)
{
	if (s_cmd[1])
	{
		errno = ENOENT;
		dprintf(2, "Minishell: %s: %s\n", s_cmd[1], strerror(errno));
		return (1);
	}
	print_env(data);
	return (0);
}
