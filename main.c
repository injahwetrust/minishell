/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:52:35 by injah             #+#    #+#             */
/*   Updated: 2023/06/12 17:59:04 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		data->pipe = 0;
		data->base_fd[0] = dup(0);
		data->base_fd[1] = dup(1);
		getcwd(data->cwd, sizeof(data->cwd));
		edit_prompt(data, data->cwd);
}

int	main(int ac, char **av, char **env) 
{
	t_data	data;
	char *input;
	int	ret;
	(void)ac;
	(void)av;
	int status;
	ft_printf(BO_GREEN HEADER RESET);
	
	init(&data, env);
	while (1) 
	{
		ret = 0;
		init_loop(&data);
		signals(1);
		input = readline(data.prompt);
		add_history(input);
		if (input == NULL)
		{
			free_all(&data, input);
			break;
		}
		else if (!ft_strcmp(input, ""))
		{
			close (data.base_fd[0]);
			close (data.base_fd[1]);
			free(data.prompt);
			free(input);
			continue;	
		}
		
		input = ft_strtrim(input, " \t", 1);
		
		edit_dollar(&data, input);
		
		while (data.dollar--)
			input = ez_money(&data, input);
		
		edit_pipe(&data, input);							//ne pas bouger l'ordre des fonctions, sinon bug =)
		
		ret = manage_nonchild(&data, input);
		if (ret == 1)
			continue;
		
		
		data.cmd = ft_split(input, '|');
		free(input);
		free(data.prompt);
		execution(&data);									//bien laisser les free avant de creer les child sinon on duplique les heaps et bug
		while (wait(&status) > 0)
			;
		if (!isatty(0))
			print(&data);
		ft_free_tab(data.cmd);
		dup2(data.base_fd[0], 0);
		close (data.base_fd[0]);
		dup2(data.base_fd[1], 1);
		close (data.base_fd[1]);
	}
	return (0);
}
