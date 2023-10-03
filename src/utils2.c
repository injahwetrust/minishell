/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 01:27:00 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/22 14:27:58 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	remove_str(char *src, char *str)
{
	int	i;
	int	j;

	i = ft_strlen(str);
	j = 0;
	while (src[i])
		src[j++] = src[i++];
	src[j] = '\0';
}

char	*get_env(t_data *data, char *macro)
{
	int		i;
	int		j;
	char	*var;

	i = 0;
	while (data->env[i])
	{
		j = 0;
		while (data->env[i][j] != '=')
			j++;
		var = ft_strndup(data->env[i], j, 0);
		if (!var)
			exit(666);
		if (ft_strcmp(var, macro) == 0)
		{
			free(var);
			return (data->env[i] + j + 1);
		}
		free(var);
		i++;
	}
	return (NULL);
}

void	is_a_path(t_data *data, char *cmd)
{
	int	i;

	i = ft_strlen(cmd);
	while (i > 0)
	{
		if (cmd[i] == '/')
			break ;
		i--;
	}
	if (i == 0)
		return ;
	if (access(cmd, F_OK))
	{
		ft_dprintf(2, "Minishell: %s: %s\n", cmd, strerror(errno));
		step0(data);
		exit(127);
	}
}

void	edit_paths(t_data *data, char *cmd)
{
	int	i;

	if (!get_env(data, "PATH"))
	{
		errno = ENOENT;
		ft_dprintf(2, "Minishell: %s: %s\n", cmd, strerror(errno));
		step0(data);
		exit(127);
	}
	is_a_path(data, cmd);
	i = -1;
	data->paths = ft_split(get_env(data, "PATH"), ':');
	if (!data->paths)
		return ;
	while (data->paths[++i])
		data->paths[i] = ft_strjoin(data->paths[i], "/", 1);
}

char	*get_exec(char *cmd, t_data *data)
{
	int	i;

	i = -1;
	if (!data->paths)
		return (cmd);
	while (data->paths[++i])
	{
		data->paths[i] = ft_strjoin(data->paths[i], cmd, 1);
		if (access(data->paths[i], F_OK | X_OK) == 0)
			return (data->paths[i]);
	}
	return (cmd);
}
