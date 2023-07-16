/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 20:30:21 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/15 02:43:45 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int here_pid;
int	main(int argc, char **argv, char **env)
{
	t_data	data;

	data.argc = argc;
	if (argc == 1)
		header();
	init(&data, argv, env);
	process(&data);
	return (0);
}
