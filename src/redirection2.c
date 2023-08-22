/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 16:38:13 by mablatie          #+#    #+#             */
/*   Updated: 2023/08/22 16:38:27 by mablatie         ###   ########.fr       */
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
