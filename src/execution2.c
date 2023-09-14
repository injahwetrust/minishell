/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 17:08:34 by mablatie          #+#    #+#             */
/*   Updated: 2023/09/14 15:44:28 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	cancel_cmd_norm(char **s_cmd)
{
	if (ft_strcmp(s_cmd[0], "\t") == 0)
	{
		free (s_cmd[0]);
		s_cmd[0] = ft_strdup("");
	}
}

void	dir_null(t_data *data, DIR *dir, char **s_cmd)
{
	if (dir != NULL)
	{
		closedir(dir);
		ft_dprintf(2, "Minishell: %s: Is a directory\n", s_cmd[0]);
		if (data->paths)
			ft_free_tab(data->paths);
		step0(data);
		exit(126);
	}
}
