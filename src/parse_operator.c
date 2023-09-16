/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vanitas <vanitas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 11:03:24 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/16 15:02:24 by vanitas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

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
		return (ft_dprintf(2, PARSE_ERR_SYCH, '('));
	else if (data->par < 0)
		return (ft_dprintf(2, PARSE_ERR_SYCH, ')'));
	return (0);
}

static int	count_op(t_data *data, char c)
{
	if (data->or == 3 && c != '|')
		return (ft_dprintf(2, PARSE_ERR_SYCH, '|'));
	else if (data->or == 3 && c == '|')
		return (ft_dprintf(2, MINI PARSE_ERR_SYSTR, "||"));
	else if (c == '|')
		data->or++;
	else
		data->or = 0;
	if (data->and == 1 && c != '&')
		return (ft_dprintf(2, PARSE_ERR_SYCH, '&'));
	if (data->and == 3 && c != '&')
		return (ft_dprintf(2, PARSE_ERR_SYCH, '&'));
	else if (data->and == 3 && c == '&')
		return (ft_dprintf(2, MINI PARSE_ERR_SYSTR, "&&"));
	else if (c == '&')
		data->and++;
	else
		data->and = 0;
	return (0);
}

static int	count_in_out(t_data *data, char c)
{
	if (data->out == 3 && c != '>')
		return (ft_dprintf(2, PARSE_ERR_SYCH, '>'));
	else if (data->out == 3 && c == '>')
		return (ft_dprintf(2, MINI PARSE_ERR_SYSTR, ">>"));
	else if (c == '>')
		data->out++;
	else if (c != ' ')
		data->out = 0;
	if (data->in == 3 && c != '<')
		return (ft_dprintf(2, PARSE_ERR_SYCH, '<'));
	else if (data->in == 4 && c != '<')
		return (ft_dprintf(2, MINI PARSE_ERR_SYSTR, "<<"));
	else if (data->in == 4 && c == '<')
		return (ft_dprintf(2, MINI PARSE_ERR_SYSTR, "<<<"));
	else if (c == '<')
		data->in++;
	else if (c != ' ')
		data->in = 0;
	return (0);
}

static int	op_newline(t_data *data)
{
	int		i;
	char	*last;

	i = ft_strlen(data->input) - 1;
	i = op_newline_norm(data, i);
	if (i == 0)
		last = ft_strdup(data->input);
	else
		last = ft_strndup(data->input, -(i + 1), 0);
	if (!last)
		end(data);
	if (!ft_strncmp(last, "&", 1) && ft_strlen(last) == 1)
		return (free(last), ft_dprintf(2, PARSE_ERR_SYCH, '&'));
	else if (!ft_strncmp(last, "&", 2) && ft_strlen(last) > 1)
		return (free(last), ft_dprintf(2, MINI PARSE_ERR_SYSTR, "&&"));
	else if (!ft_strncmp(last, "|", 1) && ft_strlen(last) == 1)
		return (free(last), ft_dprintf(2, MINI PARSE_ERR_SYSTR, "'|'"));
	else if (!ft_strncmp(last, "||", 2) && ft_strlen(last) > 1)
		return (free(last), ft_dprintf(2, MINI PARSE_ERR_SYSTR, "||"));
	else if (!ft_strcmp(last, "<") || !ft_strcmp(last, "<<")
		|| !ft_strcmp(last, "<<<") || !ft_strcmp(last, ">")
		|| !ft_strcmp(last, ">>") || !ft_strcmp(last, "<>"))
		return (free(last), ft_dprintf(2, MINI PARSE_ERR_SYSTR, "newline"));
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
