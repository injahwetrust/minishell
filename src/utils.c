/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 00:17:39 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/18 16:24:23 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	edit_lit(t_data *data, char c)
{
	if (c == '\'' && !data->d_lit)
		data->lit++;
	else if (c == '"' && !data->lit)
		data->d_lit++;
	data->lit %= 2;
	data->d_lit %= 2;
}

void	edit_par(t_data *data, char c)
{
	if (c == '(')
		data->par++;
	else if (c == ')')
		data->par--;
}

int	in_charset(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (charset[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	remove_tab(t_data *data, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		edit_lit(data, str[i]);
		if (str[i] == '#' && !is_lit(data) && i == 0)
			return (1);
		if (str[i] == '\t' && !is_lit(data))
			str[i] = ' ';
		if (str[i] == '#' && !is_lit(data) && i > 0
			&& in_charset(str[i - 1], " |&"))
		{
			str[i] = '\0';
			return (0);
		}
		i++;
	}
	data->lit = 0;
	data->d_lit = 0;
	return (0);
}

int	is_lit(t_data *data)
{
	return (data->lit + data->d_lit);
}
