/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 11:03:24 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/25 19:58:04 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_quotes_parenthesis(t_data *data)
{
	int	i;
	
	i = 0;
	while (data->input[i])
	{
		edit_lit(data, data->input[i]);
		edit_par(data, data->input[i]);
		i++;
	}
	if (data->lit)
		return (ft_dprintf(2, "Minishell Error: unclosed quotes\n"));
	else if (data->d_lit)
		return (ft_dprintf(2, "Minishell Error: unclosed double quotes\n"));
	if (data->par > 0)
		return (ft_dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", '('));
	else if (data->par < 0)
		return (ft_dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", ')'));
	//dprintf(2, "parse_quotes_parenthesis passed\n");
	return (0);
}

static int	count_op(t_data *data, char c)
{
	if (data->or == 3 && c != '|')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", '|'));
	else if (data->or == 3 && c == '|')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %s »\n", "||"));
	else if (c == '|')
		data->or++;
	else
		data->or = 0;
	if (data->and == 1 && c != '&')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", '&'));
	if (data->and == 3 && c != '&')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", '&'));
	else if (data->and == 3 && c == '&')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %s »\n", "&&"));
	else if (c == '&')
		data->and++;
	else
		data->and = 0;
	//dprintf(2, "count_op passed\n");
	return (0);
}

static int	count_in_out(t_data *data, char c)
{
	if (data->out == 3 && c != '>')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", '>'));
	else if (data->out == 3 && c == '>')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %s »\n", ">>"));
	else if (c == '>')
		data->out++;
	else
		data->out = 0;
	if (data->in == 3 && c != '<')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", '<'));
	else if (data->in == 4 && c != '<')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %s »\n", "<<"));
	else if (data->in == 4 && c == '<')
		return (dprintf(2, "Minishell: Syntax error near unexpected token « %s »\n", "<<<"));
	else if (c == '<')
		data->in++;
	else
		data->in = 0;
	//dprintf(2, "count_in_out passed\n");
	return (0);
}

static int	op_newline(t_data *data)
{
	int i;
	char	*last;
	
	i = ft_strlen(data->input) - 1;
	while (i > 0 && (data->input[i] == '<' || data->input[i] == '>' || data->input[i] == '&' || data->input[i] == '|'))
		i--;
	if (i == 0)
		last = ft_strdup(data->input);
	else
		last = ft_strndup(data->input, -(i + 1), 0);
	if (!last)
		end(data);
	if (!ft_strncmp(last, "&", 1) && strlen(last) == 1)
		return (free(last), dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", '&'));
	else if (!ft_strncmp(last, "&", 2) && strlen(last) > 1)
		return (free(last), dprintf(2, "Minishell: Syntax error near unexpected token « %s »\n", "&&"));
	else if (!ft_strncmp(last, "|", 1) && strlen(last) == 1)
		return (free(last), dprintf(2, "Minishell: Syntax error near unexpected token « %c »\n", '|'));
	else if (!ft_strncmp(last, "||", 2) && strlen(last) > 1)
		return (free(last), dprintf(2, "Minishell: Syntax error near unexpected token « %s »\n", "||"));
	else if (!ft_strcmp(last, "<") || !ft_strcmp(last, "<<")
		|| !ft_strcmp(last, "<<<") || !ft_strcmp(last, ">")
			|| !ft_strcmp(last, ">>") || !ft_strcmp(last, "<>"))
		return (free(last), dprintf(2, "Minishell: Syntax error near unexpected token « %s »\n", "newline"));
	//dprintf(2, "op_newline passed\n");
	return (free(last), 0);
}

int	parse_op(t_data *data)
{
	int	i;
	
	i = -1;
	if (parse_quotes_parenthesis(data))
		return (1);
	while (data->input[++i])
	{
		edit_lit(data, data->input[i]);
		if (!is_lit(data) && (count_op(data, data->input[i])
			|| count_in_out(data, data->input[i])))
			return (1);
		if ((data->input[i] == '|' || data->input[i] == '&'
			|| data->input[i] == '<' || data->input[i] == '>')
				&& !is_lit(data))
		if (mixed_op(data->input + i, data->input[i]))
			return (1);
	}
	edit_lit(data, data->input[i]);
	if (!is_lit(data) && (count_op(data, data->input[i])
			|| count_in_out(data, data->input[i])))
		return (1);
	if (false_space(data) || op_newline(data))
		return (1);
	return (0);
}
