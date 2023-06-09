/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:11:04 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/09 11:10:41 by bvaujour         ###   ########.fr       */
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

char	*parse_export(char *input)
{
	int	i;
	int	equal;
	char	*new;
	
	i = 0;
	equal = 0;
	new = ft_strdup(input + 6);
	if (!new)
		return (NULL);
	new = ft_strtrim(new, " \t", 1);
	if (!new)
		return (NULL);
	if (new[0] == '=')
		return (NULL);
	while (new[i] && new[i] != '=')
		i++;
	if (!new[i])
		return (NULL);
	i = 0;
	while (new[i])
	{
		if (new[i] == '=')
			equal = 1;
		if (new[i] == ' ' && equal == 0)
		{
			ft_dprintf(2, "minishell: export: wrong identifier\n");
			return (NULL);
		}
		if (new[i] == ' ' && equal == 1)
			new[i] = '\0';
		i++;
	}
	free(input);
	return (new);
}