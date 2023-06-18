/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 13:35:26 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/18 13:52:26 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		handle_slash(t_data *data, char c, int *i)
{
	if (c == '\\' && !data->d_lit && !data->lit)
	{
		if (data->slash == 0)
			data->input = ft_strremove(data->input, "\\", 1, 1);
		else
			*i += 1;
		data->slash++;
		data->slash %= 2;
		return (1);
	}
	return (0);
}

static int	handle_lit(t_data *data, char c)
{
	if (c == '"' && !data->lit)
	{
		data->input = ft_strremove(data->input, "\"", 1, 1);
		data->d_lit++;
		data->d_lit %= 2;
		return (1);
	}
	return (0);
}

static int	handle_dlit(t_data *data, char c)
{
	if (c == '\'' && !data->d_lit)
	{
		data->input = ft_strremove(data->input, "'", 1, 1);
		data->lit++;
		data->lit %= 2;
		return (1) ;
	}
	return (0);
}

void	edit_dollar(t_data *data)
{
	int	i;
	i = 0;
	while (data->input[i])
	{
		if (handle_slash(data, data->input[i], &i))
			continue ;
		if (handle_lit(data, data->input[i]))
			continue ;
		if (handle_dlit(data, data->input[i]))
			continue ;
		if (data->input[i] == '$' && data->lit == 0)
			data->dollar++;
		i++;
	}
}
