/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 13:40:58 by bvaujour          #+#    #+#             */
/*   Updated: 2023/08/25 13:54:10 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

int	is_in_env(t_data *data, char *str)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (!ft_strncmp(str, data->env[i], ft_strlen(str))
			&& data->env[i][ft_strlen(str)] == '=')
			return (1);
		i++;
	}
	i = 0;
	while (data->ghost[i])
	{
		if (!ft_strcmp(str, data->ghost[i]))
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

void	print(void)
{
	char	buf[50];
	int		ret;

	ret = 1;
	while (ret)
	{
		ret = read(0, buf, sizeof(buf));
		buf[ret] = '\0';
		write(1, buf, ret);
	}
}

int	op_newline_norm(t_data *data, int i)
{
	while (i > 0 && (data->input[i] == '<' || data->input[i] == '>'
			|| data->input[i] == '&' || data->input[i] == '|'))
		i--;
	return (i);
}

int	error_print(char *s_cmd)
{
	int	res;
	int	fd;

	fd = dup(1);
	dup2(2, 1);
	res = printf("Minishell: %s : command not found\n", s_cmd);
	dup2(fd, 1);
	return (res);
}
