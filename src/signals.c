/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 14:18:16 by vanitas           #+#    #+#             */
/*   Updated: 2023/09/16 21:14:29 by bvaujour         ###   ########.fr       */
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

static void	handler_4(int sig)
{
	kill(g_last_ret, SIGKILL);
	ft_dprintf(2, "\n");
	g_last_ret = 130;
	(void)sig;
}

void	handler_save(int sig)
{
	int		fd;
	char	*str;

	fd = open("/tmp/minishell_save", O_RDONLY);
	if (fd == -1)
		printf("\nno saved commands\n");
	else
		str = save_option(fd);
	if (str)
	{
		str = ft_strtrim(str, "\n", 1);
		rl_replace_line(str, 0);
		free(str);
	}
	else
		rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	signals(int sig)
{
	if (sig == 1)
	{
		signal(SIGINT, handler_1);
		signal(SIGTSTP, handler_save);
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
		signal(SIGINT, handler_4);
		signal(SIGQUIT, SIG_IGN);
	}
}
