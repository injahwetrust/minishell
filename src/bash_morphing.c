/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_morphing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 08:42:52 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/18 14:43:49 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	is_it_exec(t_data *data, char *cmd)
{
	char	**paths;
	int		i;

	i = -1;
	paths = ft_split(get_env(data, "PATH"), ':');
	while (paths[++i])
	{
		paths[i] = ft_strjoin(paths[i], "/", 1);
		paths[i] = ft_strjoin(paths[i], cmd, 1);
		if (access(paths[i], F_OK | X_OK) == 0)
		{
			ft_free_tab(paths);
			return (1);
		}
	}
	ft_free_tab(paths);
	return (0);
}

static void	env_and_cmd(t_data *data, t_cmd *cmds)
{
	int		i;
	char	**new;

	i = 0;
	while (ft_strcmp(cmds->s_cmd[i], "env") == 0)
	{
		i++;
		if (cmds->s_cmd[i] && ft_strcmp(cmds->s_cmd[i], "env")
			&& is_it_exec(data, cmds->s_cmd[i]))
		{
			new = ft_tabdup(cmds->s_cmd + i, NULL, 0);
			ft_free_tab(cmds->s_cmd);
			cmds->s_cmd = new;
			return ;
		}
	}
}

void	bash_morphing(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		env_and_cmd(data, &data->cmds[i]);
		i++;
	}
}
