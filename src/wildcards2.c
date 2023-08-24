/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 14:50:21 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/22 16:21:59 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	front(t_data *data, char *ast, char *name)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (ast[++i] != '*')
	{
		if (cont(data, ast[i]))
			continue ;
		if (name[j] != ast[i])
			return (0);
		j++;
	}
	return (1);
}

static int	back(t_data *data, char *ast, char *name)
{
	int	i;
	int	j;

	i = ft_strlen(ast);
	j = ft_strlen(name);
	while (ast[i] != '*')
	{
		if (cont(data, ast[i]))
		{
			i--;
			continue ;
		}
		if (ast[i] != name[j])
			return (0);
		i--;
		j--;
	}
	return (1);
}

static int	match_ast(t_data *data, char *ast, char *name)
{
	int	ret;

	ret = front(data, ast, name) + back(data, ast, name);
	data->d_lit = 0;
	data->lit = 0;
	if (ret == 2)
		return (1);
	else
		return (0);
}

static char	*replace_ast(t_data *data, char *ast)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*str;

	str = ft_strdup("");
	dir = opendir(".");
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (match_ast(data, ast, entry->d_name))
		{
			str = ft_strjoin(str, ft_strdup(entry->d_name), 3);
			str = ft_strjoin(str, " ", 1);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (ft_strcmp(str, "") == 0)
	{
		free (str);
		return (ast);
	}
	free (ast);
	return (str);
}

char	*seg_dir(t_data *data, char *new, int i)
{
	char	*ast;

	while (i > 0)
	{
		edit_lit(data, data->input[i]);
		if (data->input[i] == ' ' && !is_lit(data))
			break ;
		i--;
	}
	if (i)
		i++;
	new[i] = '\0';
	ast = get_ast(data, data->input + i);
	if (!ast)
	{
		free(new);
		end(data);
	}
	ast = replace_ast(data, ast);
	new = ft_strjoin(new, ast, 3);
	return (new);
}
