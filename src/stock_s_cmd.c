/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_s_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 08:51:24 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/20 13:28:15 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	s_cmd_count(t_data *data, char *cmd)
{
	int		count;
	int		i;
	char	*clone;

	clone = cmd;
	count = 1;
	i = 0;
	while (clone[i])
	{
		edit_lit(data, clone[i]);
		edit_par(data, clone[i]);
		if (clone[i] == ' ' && !is_lit(data) && !data->par)
			count++;
		i++;
	}
	return (count);
}

static void	split_cmd(t_data *data, char **s_cmd, char *cmd)
{
	int		i;
	int		j;
	char	*clone;

	i = 0;
	j = 0;
	clone = cmd;
	while (clone[i])
	{
		edit_lit(data, clone[i]);
		edit_par(data, clone[i]);
		if (clone[i] == ' ' && !is_lit(data) && !data->par)
		{
			s_cmd[j] = ft_strndup(clone, i, 0);
			while (clone[i] == ' ')
				i++;
			clone += i;
			i = 0;
			j++;
		}
		else
			i++;
	}
	s_cmd[j] = ft_strndup(clone, i, 0);
	s_cmd[j + 1] = 0;
}

void	fill_s_cmd(t_data *data)
{
	int	i;
	int	len;

	i = 0;
	while (i < data->count)
	{
		if (data->cmds[i].cmd[0] == '(')
		{
			data->cmds[i].cmd[0] = ' ';
			data->cmds[i].cmd[ft_strlen(data->cmds[i].cmd) - 1] = '\0';
			data->cmds[i].cmd = ft_strjoin("./minishell", data->cmds[i].cmd, 2);
		}
		len = s_cmd_count(data, data->cmds[i].cmd) + 1;
		data->cmds[i].s_cmd = malloc(sizeof(char *) * len);
		if (!data->cmds[i].s_cmd)
			exit(666);
		split_cmd(data, data->cmds[i].s_cmd, data->cmds[i].cmd);
		i++;
	}
}
