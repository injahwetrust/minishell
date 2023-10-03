/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:38:13 by mablatie          #+#    #+#             */
/*   Updated: 2023/09/22 14:29:35 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	create_outfiles(t_cmd *ccmd)
{
	int		i;
	int		j;
	int		fd;
	char	*clone;

	i = -1;
	while (ccmd->out[++i])
	{
		clone = ccmd->out[i];
		j = 0;
		while (clone[j] == '>')
			j++;
		clone += j;
		while (*clone == ' ')
			clone++;
		if (j == 1)
			fd = open(clone, O_CREAT | O_TRUNC, 0644);
		else if (j == 2)
			fd = open(clone, O_CREAT | O_APPEND, 0644);
		if (fd != -1)
			close(fd);
	}
}

void	creation(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->count)
		create_outfiles(&data->cmds[i]);
}

void	redirect_close(t_data *data)
{
	if (dup2(data->fd.base_fd[0], 0) == -1)
		(perror("Minishell"), end(data));
	if (close(data->fd.p_fd[0]) == -1)
		(perror("Minishell"), end(data));
	if (dup2(data->fd.p_fd[1], 1) == -1)
		(perror("Minishell"), end(data));
	if (close(data->fd.p_fd[1]) == -1)
		(perror("Minishell"), end(data));
}

char	*manage_dollar_heredoc(t_data *data, char *here)
{
	int		i;
	int		dol;
	char	*str;

	dol = 0;
	i = -1;
	while (here[++i])
	{
		edit_lit(data, here[i]);
		if (here[i] == '$' && !data->lit
			&& (in_charset(here[i + 1], data->ex)
				|| in_charset(here[i + 1], "\"'?")))
			dol++;
	}
	while (dol)
	{
		str = str_dollar(data, here);
		free(here);
		here = ft_strdup(str);
		free(str);
		dol--;
		data->d_lit = 0;
		data->lit = 0;
	}
	return (here);
}
