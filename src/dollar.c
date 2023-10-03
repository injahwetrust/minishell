/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 17:51:27 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/22 14:24:38 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static char	*join_dollar(t_data *data, char *begin, char *macro)
{
	char	*str;

	if (ft_strcmp(get_env(data, macro), "\"") == 0 && data->d_lit)
		str = ft_strjoin(begin, "\"'\"'\"", 1);
	else
	{
		str = ft_strjoin(begin, "\"", 1);
		str = ft_strjoin(str, get_env(data, macro), 1);
		str = ft_strjoin(str, "\"", 1);
	}
	return (str);
}

int	get_part_norm(t_data *data, char *part, int i)
{
	while (part[++i] && ((in_charset(part[i], data->ex)
				&& !in_charset(part[i], "\"'")) || part[i] == '?'))
		if (i == 1 && (in_charset(part[i - 1], "0123456789")
				|| part[i - 1] == '?'))
			break ;
	return (i);
}

static char	*get_part(t_data *data, char *begin, char *part)
{
	int		i;
	char	*macro;
	char	*str2;

	i = -1;
	i = get_part_norm(data, part, i);
	macro = ft_strndup(part, i, 0);
	if (ft_strcmp(macro, "") == 0)
		str2 = ft_strjoin(begin, part, 1);
	else if (ft_strncmp(macro, "?", 1) == 0)
	{
		str2 = ft_strjoin(begin, ft_itoa(data->heredoc_ret), 3);
		str2 = ft_strjoin(str2, ft_strndup(part, -i, 0), 3);
	}
	else if (get_env(data, macro))
	{
		str2 = join_dollar(data, begin, macro);
		str2 = ft_strjoin(str2, ft_strndup(part, -i, 0), 3);
	}
	else
		str2 = ft_strjoin(begin, ft_strndup(part, -i, 0), 3);
	free(macro);
	return (str2);
}

char	*str_dollar(t_data *data, char *str)
{
	int		i;
	char	*replaced;

	i = -1;
	data->lit = 0;
	data->d_lit = 0;
	while (str[++i])
	{
		edit_lit(data, str[i]);
		if (str[i] == '$' && !data->lit && !in_charset(str[i + 1], data->ex)
			&& !in_charset(str[i + 1], "\"'?") && str[i + 1])
			continue ;
		else if (str[i] == '$' && !data->lit && i != 0)
		{
			replaced = get_part(data, ft_strndup(str, i, 0), str + i + 1);
			return (replaced);
		}
		else if (str[i] == '$' && !data->lit && i == 0)
		{
			replaced = get_part(data, ft_strdup(""), str + i + 1);
			return (replaced);
		}
	}
	return (NULL);
}

void	manage_dollar(t_data *data)
{
	int		i;
	int		dol;
	char	*str;

	dol = 0;
	i = -1;
	while (data->input[++i])
	{
		edit_lit(data, data->input[i]);
		if (data->input[i] == '$' && !data->lit
			&& (in_charset(data->input[i + 1], data->ex)
				|| in_charset(data->input[i + 1], "\"'?")))
			dol++;
	}
	while (dol)
	{
		str = str_dollar(data, data->input);
		free(data->input);
		data->input = ft_strdup(str);
		free(str);
		dol--;
		data->d_lit = 0;
		data->lit = 0;
	}
}
