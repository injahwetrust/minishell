/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 19:47:15 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/18 16:27:02 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static char	*all_dir(t_data *data)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*str;

	str = ft_strdup("");
	dir = opendir(".");
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
		{
			str = ft_strjoin(str, ft_strdup(entry->d_name), 3);
			if (!str)
				end(data);
			str = ft_strjoin(str, " ", 1);
			if (!str)
				end(data);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (ft_strcmp(str, "") == 0)
		return (ft_strdup("*"));
	return (str);
}

char	*get_ast(t_data *data, char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != ' ')
	{
		edit_lit(data, input[i]);
		i++;
	}
	return (ft_strndup(input, i, 0));
}

static int	is_all_dir(t_data *data, int i)
{
	while (i > 0)
	{
		edit_lit(data, data->input[i]);
		if (data->input[i] == ' ' && !is_lit(data))
			break ;
		i--;
	}
	if (i)
		i++;
	while (data->input[i] != '*')
	{
		if (data->input[i] != '"' && data->input[i] != '\'')
			return (0);
		i++;
	}
	while (data->input[i] == '*')
		i++;
	while (data->input[i] && data->input[i] != ' ')
	{
		if (data->input[i] != '"' && data->input[i] != '\'')
			return (0);
		i++;
	}
	return (1);
}

static int	pass_the_word(t_data *data, int i)
{
	while (data->input[i])
	{
		edit_lit(data, data->input[i]);
		if (data->input[i] == ' ' && !is_lit(data))
			break ;
		i++;
	}
	i--;
	return (i);
}

char	*wildcards(t_data *data)
{
	int		i;
	char	*new;

	i = -1;
	new = ft_strdup("");
	if (!new)
		end(data);
	while (data->input[++i])
	{
		edit_lit(data, data->input[i]);
		if (data->input[i] == '*' && !is_lit(data) && is_all_dir(data, i))
			new = ft_strjoin(new, all_dir(data), 3);
		else if (data->input[i] == '*' && !is_lit(data) && !is_all_dir(data, i))
		{
			new = seg_dir(data, new, i);
			i = pass_the_word(data, i);
		}
		else
			new = ft_strjoin(new, ft_strndup(data->input + i, 1, 0), 3);
	}
	free(data->input);
	return (new);
}
