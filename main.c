/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:52:35 by injah             #+#    #+#             */
/*   Updated: 2023/06/14 01:06:37 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

void	init(t_data *data, char **env)
{
	data->env = ft_tabdup(env);
	data->ex = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
	data->wrong_char = "!$%%&*()";
	edit_paths(data);
}

void	init_loop(t_data *data)
{
		data->dollar = 0;
		data->heredoc = 0;
		data->append = 0;
		unlink("heredoc");
		data->pipe = 0;
		data->fd.base_fd[0] = dup(0);
		data->fd.base_fd[1] = dup(1);
		getcwd(data->cwd, sizeof(data->cwd));
		edit_prompt(data, data->cwd);
}

void	header(void)
{
	struct winsize win;
	int	i;
	int	marge;
	
	marge = ft_strlen(HEADER1) / 2;
	i = -1;
	ioctl(1, TIOCGWINSZ, &win);
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(BO_RED HEADER1);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(BO_RED HEADER2);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(BO_RED HEADER3);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(BO_RED HEADER4);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(BO_RED HEADER5 RESET);
	
}

int	main(int ac, char **av, char **env) 
{
	t_data	data;
	int	ret;
	(void)ac;
	(void)av;
	int status;

	
	header();
	init(&data, env);
	while (1) 
	{
		ret = 0;
		init_loop(&data);
		signals(1);
		data.input = readline(data.prompt);
		add_history(data.input);
		if (data.input == NULL)
			free_all(&data);
		else if (!ft_strcmp(data.input, ""))
		{
			close (data.fd.base_fd[0]);
			close (data.fd.base_fd[1]);
			free(data.prompt);
			free(data.input);
			continue;	
		}
		
		data.input = ft_strtrim(data.input, " \t", 1);
		
		edit_dollar(&data);
		
		while (data.dollar--)
			data.input = ez_money(&data);
		
		edit_pipe(&data);							//ne pas bouger l'ordre des fonctions, sinon bug =)
		
		ret = manage_nonchild(&data);
		if (ret == 1)
			continue;
		data.cmd = ft_split(data.input, '|');
		free(data.input);
		free(data.prompt);
		execution(&data);									//bien laisser les free avant de creer les child sinon on duplique les heaps et bug
		while (wait(&status) > 0)
			;
		data.last_ret = WEXITSTATUS(status);
		if (!isatty(0))
			print(&data);
		ft_free_tab(data.cmd);
		dup2(data.fd.base_fd[0], 0);
		close (data.fd.base_fd[0]);
		dup2(data.fd.base_fd[1], 1);
		close (data.fd.base_fd[1]);
	}
	return (0);
}
