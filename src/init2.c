/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vanitas <vanitas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:51:46 by vanitas           #+#    #+#             */
/*   Updated: 2023/09/16 14:54:34 by vanitas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	init_norm(t_data *data, char **env)
{
	g_last_ret = 0;
	data->active_ret = -1;
	data->step1 = 0;
	data->last_cmd = ft_strdup("./minishell");
	if (!data->last_cmd)
		step1(data);
	data->step1 = 1;
	data->ghost = malloc(sizeof(char *));
	if (!data->ghost)
		step1(data);
	data->ghost[0] = 0;
	data->step1 = 2;
	data->ex = AZ_MIN DATA_EX;
	(init_env(data, env), add_in_env(data, "_=/usr/bin/env"));
}
