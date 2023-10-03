/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 23:55:59 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/21 15:22:05 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

int	init_env(t_data *data, char **env)
{
	char	*shell;
	int		a;

	data->env = ft_tabdup(env, NULL, 0);
	if (!data->env)
		step1(data);
	data->step1 = 3;
	shell = get_env(data, "SHLVL");
	if (shell)
	{
		a = ft_atoi(shell);
		a++;
		shell = ft_strjoin("SHLVL=", ft_itoa(a), 2);
		if (!shell)
			step1(data);
		replace_in_env(data, shell);
	}
	else
		add_in_env(data, "SHLVL=1");
	remove_from_env(data, "OLDPWD");
	free(shell);
	return (0);
}

void	init(t_data *data, char **argv, char **env)
{
	int	i;

	data->fd.base_fd[0] = dup(0);
	data->fd.base_fd[1] = dup(1);
	if (data->fd.base_fd[0] == -1 || data->fd.base_fd[1] == -1)
		(perror("Minishell"), exit(0));
	i = 0;
	if (data->argc > 1)
	{
		data->input = ft_strdup("");
		while (++i < data->argc)
		{
			data->input = ft_strjoin(data->input, argv[i], 1);
			data->input = ft_strjoin(data->input, " ", 1);
			if (!data->input)
			{
				(close(data->fd.base_fd[0]), close(data->fd.base_fd[1]));
				exit (1);
			}
		}
	}
	init_norm(data, env);
}

void	edit_prompt(t_data *data, char *cwd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cwd[i])
		i++;
	while (cwd[i] != '/' && i > 0)
		i--;
	while (j < i)
	{
		cwd++;
		j++;
	}
	data->prompt = ft_strjoin(BGO_GREEN BO_BLACK"Minishell~",
			RESET BO_GREEN, 0);
	data->prompt = ft_strjoin(data->prompt, cwd, 1);
	data->prompt = ft_strjoin(data->prompt, RESET"$ ", 1);
}

int	init_loop(t_data *data)
{
	char	*pwd;

	dup2(data->fd.base_fd[0], 0);
	dup2(data->fd.base_fd[1], 1);
	getcwd(data->cwd, sizeof(data->cwd));
	pwd = ft_strjoin("PWD=", data->cwd, 0);
	if (!pwd)
		step1(data);
	add_in_env(data, pwd);
	free(pwd);
	edit_prompt(data, data->cwd);
	data->lit = 0;
	data->d_lit = 0;
	data->par = 0;
	data->and = 0;
	data->or = 0;
	data->in = 0;
	data->out = 0;
	data->step = 0;
	data->count = 1;
	data->last_pid = -1;
	data->print = 0;
	return (0);
}

void	init_cmds(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		data->cmds[i].cmd = NULL;
		data->cmds[i].s_cmd = 0;
		data->cmds[i].in = 0;
		data->cmds[i].out = 0;
		data->cmds[i].prev_op = NULL;
		data->cmds[i].next_op = NULL;
		i++;
	}
}
