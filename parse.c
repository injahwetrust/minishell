/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 13:35:26 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/26 14:04:29 by bvaujour         ###   ########.fr       */
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

static int	handle_dlit(t_data *data, char c, int j)
{
	if (c == '"' && !data->lit)
	{
		ft_printf("removed %c n %d, lit = %d, d_lit = %d\n", '"', data->lit, data->double_behind, data->d_lit);
		data->cmd[j] = ft_strremove(data->cmd[j], "\"", 1 + data->double_behind, 1);
		printf("new cmd = %s\n", data->cmd[j]);
		data->d_lit += 1;
		data->d_lit %= 2;
		return (1);
	}
	if (c == '"' && data->lit)
		data->double_behind++;
	return (0);
}

static int	handle_lit(t_data *data, char c, int j)
{
	if (c == '\'' && !data->d_lit)
	{
		ft_printf("removed %c n %d, lit = %d, d_lit = %d\n", '\'', data->lit, data->simple_behind, data->d_lit);
		data->cmd[j] = ft_strremove(data->cmd[j], "'", 1 + data->simple_behind, 1);
		printf("new cmd = %s\n", data->cmd[j]);
		data->lit += 1;
		data->lit %= 2;
		return (1) ;
	}
	if (c == '\'' && data->d_lit)
		data->simple_behind++;
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
		data->lit = 0;
		data->d_lit = 0;
		data->simple_behind = 0;
		data->double_behind = 0;
		i = 0;
		while (data->cmd[j][i])
		{
			//printf("step = %c\n", data->cmd[j][i]);
			if (handle_lit(data, data->cmd[j][i], j))
				continue ;
			if (handle_dlit(data, data->cmd[j][i], j))
				continue ;
			i++;
		}
		j++;
	}
}

void	edit_dollar(t_data *data)
{
	int	i;
	
	i = 0;
	data->lit = 0;
	data->d_lit = 0;
	while (data->input[i])
	{
		if (data->input[i] == '\'' && data->d_lit == 0)
			data->lit++;
		data->lit %= 2;
		if (data->input[i] == '"' && data->lit == 0)
			data->d_lit++;
		data->d_lit %= 2;
		if (data->input[i] == '$' && data->lit == 0)
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
	char	*part;
	
	i = 0;
	data->lit = 0;
	data->d_lit = 0;
	while (data->input[i])
	{
		if (data->input[i] == '\'')
			data->lit++;
		else if (data->input[i] == '"')
			data->d_lit++;
		else if ((data->input[i] == '|' || data->input[i] == '&') && !data->lit && !data->d_lit)
		{
			part = ft_strndup(data->input, i, 0);
			data->input = ft_strremove(data->input, part, 1, 1);
			return (part);
		}
		data->lit %= 2;
		data->d_lit %= 2;
		i++;
	}
	part = ft_strdup(data->input);
	free(data->input);
	data->input = ft_strdup("");
	return (part);
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
	//printf("cmd count = %d\n", i);
	data->cmd = malloc(sizeof(char *) * (i + 1));
	if (!data->cmd)
		free_all(data);
	data->cmd[i] = 0;
	if (!*data->input)
	{
		data->cmd[0] = ft_strdup("");
		return ;
	}
	data->ope = malloc(sizeof(char *) * i);
	if (!data->ope)
		free_all(data);
	data->ope[i - 1] = 0;
	i = 0;
	while (*data->input)
	{
		//printf("boucle parse input\n");
		data->cmd[i] = input_part(data);
		if (*data->input)
		{
		  	data->ope[i] = get_op(data);
		 	if (!*data->input)
		 		data->cmd[i + 1] = ft_strdup("\t");
		}
		i++;
	}
	// i = -1;
	// while (data->cmd[++i])
	// 	ft_printf("cmd[%d] = :%s:\n", i, data->cmd[i]);
	// i = -1;
	// while (data->ope[++i])
	// 	ft_printf("ope[%d] = :%s:\n", i, data->ope[i]);
}
