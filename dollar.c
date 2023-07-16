/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 17:51:27 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/16 16:52:47 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_part(t_data *data, char *begin, char *part)
{
	int	i;
	char	*macro;
	char	*str2;

	i = 0;
	while (part[i] && part[i] != ' ' && part[i] != '\'' && part[i] != '"')
		i++;
	macro = ft_strndup(part, i, 0);
	if (ft_strcmp(macro, "?") == 0)
	{
		str2 = ft_strjoin(begin, ft_itoa(data->last_ret), 3);
		str2 = ft_strjoin(str2, ft_strndup(part, -i, 0), 3);
	}
	else if (get_env(data, macro))
	{
		str2 = ft_strjoin(begin, get_env(data, macro), 1);
		str2 = ft_strjoin(str2, ft_strndup(part, -i, 0), 3);
	}
	else
		str2 = ft_strjoin(begin, ft_strndup(part, -i, 0), 3);
	free(macro);
	return (str2);
}

static char *str_dollar(t_data *data, char *str)
{
	int	i;
	char *replaced;
	i = 0;
	printf("entree dans strdoll = %s\n", str);
	data->lit = 0;
	data->d_lit = 0;
	while (str[i])
	{
		edit_lit(data, str[i]);
		if (str[i] == '$' && !data->lit && i != 0)
		{
			replaced = get_part(data, ft_strndup(str, i, 0), str + i + 1);
			return (replaced);
		}
		else if (str[i] == '$' && !data->lit && i == 0)
		{
			replaced = get_part(data, ft_strdup(""), str + i + 1);
			return (replaced);
		}
		i++;
	}
	return (NULL);
}

static void	replace_str(t_data *data, char **s_cmd, int i)
{
	char	*str;
	int	j;
	int	dol;
	
	dol = 0;
	j = -1;
	while (s_cmd[i][++j])
	{
		edit_lit(data, s_cmd[i][j]);
		if (s_cmd[i][j] == '$' && !data->lit)
			dol++;
	}
	while (dol)
	{
		str = str_dollar(data, s_cmd[i]);
		free(s_cmd[i]);
		s_cmd[i] = ft_strdup(str);
		free(str);
		dol--;
	}
}

static void	replace_dollar(t_data *data, char **s_cmd)
{
	int	i;


	i = 0;
	while (s_cmd[i])
	{
		replace_str(data, s_cmd, i);
		i++;
	}
}

void	manage_dollar(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		replace_dollar(data, data->cmds[i].s_cmd);
		i++;
	}
	
}
