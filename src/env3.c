/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 00:29:58 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/15 00:38:17 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	check_ghost(t_data *data, char *str)
{
	int		i;
	char	*part;
	(void)data;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	part = ft_strndup(str, i, 0);
	if(is_in_env(data, part))
		remove_from_ghost(data, part);
	free(part);
}