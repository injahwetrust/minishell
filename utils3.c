/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 13:40:58 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/26 15:46:44 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_in_env(t_data *data, char *str)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (!strncmp(str, data->env[i], ft_strlen(str)) && data->env[i][ft_strlen(str)] == '=')
			return (1);
		i++;
	}
	i = 0;
	while (data->ghost[i])
	{
		if (!strcmp(str, data->ghost[i]))
			return (2);
		i++;
	}
	return (0);
}

int	cont(t_data *data, char c)
{
	if ((c == '"' || c == '\'') && !is_lit(data))
	{
		edit_lit(data, c);
		return (1);
	}
	if (c == '"' && data->d_lit)
	{
		edit_lit(data, c);
		return (1);
	}
	if (c == '\'' && data->lit)
	{
		edit_lit(data, c);
		return (1);
	}
	return (0);
}

