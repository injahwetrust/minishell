/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 20:30:21 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/15 09:58:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/minishell.h"

int	g_last_ret;

void	toggleSignalsOn(void)
{
	struct termios		terminal;
 
	tcgetattr(1, &terminal);
	terminal.c_lflag &= ~ECHOCTL;
	tcsetattr(1, TCSANOW, &terminal);
}

void	toggleSignalsOff(void)
{
	struct termios		terminal;
 
	tcgetattr(1, &terminal);
	terminal.c_lflag |= ECHOCTL;
	tcsetattr(1, TCSANOW, &terminal);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	if ((!isatty(1) || !isatty(0)) && argc == 1)
	{
		ft_dprintf(2, "Abord\n");
		return (0);
	}
	data.argc = argc;
	if (argc == 1)
		header();
	init(&data, argv, env);
	process(&data);
	return (0);
}
