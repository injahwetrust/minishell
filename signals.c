/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 14:18:16 by vanitas           #+#    #+#             */
/*   Updated: 2023/06/17 17:18:10 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signals(t_data *data, int sig)
{
	(void) data;
	if (sig == 1)
	{
		signal(SIGINT, handler_1);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 2)
	{
		signal(SIGINT, handler_2);
		signal(SIGQUIT, handler_back_slash);
	}
	if (sig == 4)
	{
		signal(SIGINT, handler_2);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 3)
		exit (0);
}

void	handler_1(int sig)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	handler_2(int sig)
{
	write(1, "\n", 1);
	(void)sig;
}

void	handler_back_slash(int sig)
{
	ft_printf("Quit (core dumped)\n");
	(void)sig;
}