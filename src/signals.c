/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 14:18:16 by vanitas           #+#    #+#             */
/*   Updated: 2023/09/22 18:17:15 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static void	handler_1(int sig)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_last_ret = 130;
	(void)sig;
}

static void	handler_2(int sig)
{
	write(1, "\n", 1);
	g_last_ret = 130;
	(void)sig;
}

static void	handler_back_slash(int sig)
{
	ft_printf("Quit (core dumped)\n");
	g_last_ret = 131;
	(void)sig;
}

// static void	handler_4(int sig)
// {
// 	// kill(g_last_ret, SIGKILL);
// 	//close(0);
// 	ft_dprintf(2, "\n");
// 	g_last_ret = 130;
// 	(void)sig;
// }

static void	handler_5(int sig)
{
	g_last_ret = 130;
	ft_dprintf(2, "\n");
	close(0);
	(void)sig;
}

void	signals(int sig)
{
	if (sig == 1)
	{
		signal(SIGINT, handler_1);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 2)
	{
		signal(SIGINT, handler_2);
		signal(SIGQUIT, handler_back_slash);
	}
	if (sig == 3)
	{
		signal(SIGINT, handler_5);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 4)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}
