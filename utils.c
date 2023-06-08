/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:11:04 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/08 22:13:23 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	end_process(t_data *data)
{
	ft_free_tab(data->cmd);
	ft_free_tab(data->paths);
	exit(0);
}

void	ft_free_tab(char **tab)
{
	int	i;
	
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

void	close_n_dup(t_data *data)
{
	close(data->base_fd[0]);
	close(data->base_fd[1]);
	close(data->p_fd[0]);
	dup2(data->p_fd[1], 1);
	close(data->p_fd[1]);
}
