/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 14:18:16 by vanitas           #+#    #+#             */
/*   Updated: 2023/07/17 12:07:04 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handler_1(int sig)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

static void	handler_2(int sig)
{
	write(1, "\n", 1);
	(void)sig;
}

static void	handler_back_slash(int sig)
{
	ft_printf("Quit (core dumped)\n");
	(void)sig;
}

static void	handler_3(int sig)
{
	kill(here_pid, SIGKILL);
	dprintf(2, "\n");
	(void)sig;
}

void	signals(int sig)
{
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
	if (sig == 3)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 4)
	{
		signal(SIGINT, handler_3);
		signal(SIGQUIT, SIG_IGN);
	}
}
