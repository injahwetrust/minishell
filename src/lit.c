/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 23:55:37 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/18 15:14:46 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static char	*remove_quotes(t_data *data, char *cmd)
{
	int		i;
	char	*new;

	i = -1;
	new = ft_strdup("");
	while (cmd[++i])
	{
		edit_lit(data, cmd[i]);
		if (cmd[i] == '\'' && !data->d_lit)
			continue ;
		else if (cmd[i] == '"' && !data->lit)
			continue ;
		else
			new = ft_strjoin(new, ft_strndup(cmd + i, 1, 0), 3);
	}
	return (new);
}

static void	remove_lit(t_data *data, char **s_cmd)
{
	char	*str;
	int		i;

	i = 0;
	while (s_cmd[i])
	{
		if (!ft_strcmp(s_cmd[i], "\"\"") || !ft_strcmp(s_cmd[i], "''"))
		{
			free (s_cmd[i]);
			s_cmd[i] = ft_strdup("\t");
		}
		else
		{
			str = remove_quotes(data, s_cmd[i]);
			free (s_cmd[i]);
			s_cmd[i] = ft_strdup(str);
			free(str);
		}
		i++;
	}
}

void	manage_lit(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		remove_lit(data, data->cmds[i].s_cmd);
		i++;
	}
}
