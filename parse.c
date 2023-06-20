/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 13:35:26 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/21 01:10:54 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static int		handle_slash(t_data *data, char c, int *i)
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
}*/

static int	handle_lit(t_data *data, char c, int j)
{
	if (c == '"' && !data->lit)
	{
		data->cmd[j] = ft_strremove(data->cmd[j], "\"", 1, 1);
		data->d_lit++;
		data->d_lit %= 2;
		return (1);
	}
	return (0);
}

static int	handle_dlit(t_data *data, char c, int j)
{
	if (c == '\'' && !data->d_lit)
	{
		data->cmd[j] = ft_strremove(data->cmd[j], "'", 1, 1);
		data->lit++;
		data->lit %= 2;
		return (1) ;
	}
	return (0);
}

void	manage_lit(t_data *data)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	while (data->cmd[j])
	{
		i = 0;
		while (data->cmd[j][i])
		{
			// if (handle_slash(data, data->input[i], &i))
			// 	continue ;
			if (handle_lit(data, data->cmd[j][i], j))
				continue ;
			if (handle_dlit(data, data->cmd[j][i], j))
				continue ;
			i++;
			if (!data->cmd[j][i] && data->d_lit == 1)
			{
				while (data->cmd[j][i] != '"')
					i--;
				data->cmd[j][i] = ' ';
				data->d_lit = 0;
				i = ft_strlen(data->cmd[j]);
			}
			if (!data->cmd[j][i] && data->lit == 1)
			{
				while (data->cmd[j][i] != '\'')
					i--;
				data->cmd[j][i] = ' ';
				data->lit = 0;
				i = ft_strlen(data->cmd[j]);
			}
		}
		j++;
	}
}

void	edit_dollar(t_data *data)
{
	int	i;
	
	i = 0;
	while (data->cmd[0][i])
	{
		if (data->cmd[0][i] == '$' && data->lit == 0)
			data->dollar++;
		i++;
	}
}

int	part_count(t_data *data)
{
	int	i;
	int	lit;
	int	d_lit;
	int	count;
	
	i = 0;
	lit = 0;
	d_lit = 0;
	count = 1;
	while (data->input[i])
	{
		if (data->input[i] == '\'')
			lit++;
		else if (data->input[i] == '"')
			d_lit++;
		else if (data->input[i] == '|' && !lit && !d_lit)
		{
			while (data->input[i] == '|')
				i++;
			count++;
			if (!data->input[i])
				break ;
		}
		lit %= 2;
		d_lit %= 2;
		i++;
	}
	return (count);	
}

char	*input_part(t_data *data)
{
	int	i;
	int	lit;
	int	d_lit;
	char	*part;
	
	i = 0;
	lit = 0;
	d_lit = 0;
	while (data->input[i])
	{
		if (data->input[i] == '\'')
			lit++;
		else if (data->input[i] == '"')
			d_lit++;
		else if ((data->input[i] == '|' || data->input[i] == '&') && !lit && !d_lit)
		{
			part = ft_strndup(data->input, i, 0);
			data->input = ft_strremove(data->input, part, 1, 1);
			return (part);
		}
		lit %= 2;
		d_lit %= 2;
		i++;
	}
	if (data->input[i] == '\0' && !lit && !d_lit)
	{
		part = ft_strndup(data->input, i, 0);
		data->input = ft_strremove(data->input, part, 1, 1);
		return (part);
	}
	printf("HRLLOE\n");
	return (NULL);
}

char	*get_op(t_data *data)
{
	char	*op;
	
	if (data->input[0] == data->input[1])
	{
		op = ft_strndup(data->input, 2, 0);
		data->input = ft_strremove(data->input, ft_strndup(data->input, 2, 0), 1, 3);
		return (op);
	}
	op = ft_strndup(data->input, 1, 0);
	data->input = ft_strremove(data->input, ft_strndup(data->input, 1, 0), 1, 3);
	return (op);
}

void	space(t_data *data)
{
	int	i;
	
	i = 0;
	while (data->input[i])
	{
		while (data->input[i] == '\t')
			data->input[i] = ' ';
		i++;
	}
}

void	parse_input(t_data *data)
{
	int	i;
	
	space(data);
	i = part_count(data);
	printf("count = %d\n", i);
	data->cmd = malloc(sizeof(char *) * (i + 1));
	if (!data->cmd)
		free_all(data);
	data->cmd[i] = 0;
	data->ope = malloc(sizeof(char *) * i);
	if (!data->ope)
		free_all(data);
	data->ope[i - 1] = 0;
	i = 0;
	while (*data->input)
	{
		data->cmd[i] = input_part(data);
		if (*data->input)
		{
		  	data->ope[i] = get_op(data);
		 	if (!*data->input)
		 		data->cmd[i + 1] = ft_strdup("\t");
		}
		i++;
	}
	i = -1;
	while (data->cmd[++i])
		ft_printf("cmd[%d] = :%s:\n", i, data->cmd[i]);
	i = -1;
	while (data->ope[++i])
		ft_printf("ope[%d] = :%s:\n", i, data->ope[i]);
}
