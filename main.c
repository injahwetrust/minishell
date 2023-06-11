/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:52:35 by injah             #+#    #+#             */
/*   Updated: 2023/06/12 01:36:54 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_in(char *cmd)
{
	int	i;
	char	*file;

	i = 1;
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
		i++;
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '<')
		i++;
	file = ft_strndup(cmd, i, 0);
	if (!file)
		exit(0); // faire une fonction pour exit proprement
	return (file);
}

char	*get_path(char *cmd)
{
	int	i;
	char	*file;

	i = 1;
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
		i++;
	file = ft_strndup(cmd, -i, 0);
	if (!file)
		exit(0); // faire une fonction pour exit proprement
	i = 0;
	while (file[i] && file[i] != ' ' && file[i] != '\t' && file[i] != '<' && file[i] != '>')
		i++;
	file = ft_strndup(file, i, 1);
	if (!file)
		exit(0); // faire une fonction pour exit proprement
	return (file);
}

char	*redir_in(t_data *data, char *cmd, int *redir_fd)
{
	int	i;
	char	*untrim;
	char	*path;
	char	*new;
	(void)data;
	
	i = 0;
	while (cmd[i] && cmd[i] != '<')
		i++;
	untrim = get_in(cmd + i);
	ft_dprintf(2, "untrim = %s\n", untrim);
	path = get_path(untrim);
	ft_dprintf(2, "path = %s\n", path);
	close(redir_fd[0]);
	redir_fd[0] = open(path, O_RDONLY, 0644);
	new = ft_strremove(cmd, untrim, 1, 1);
	free(untrim);
	if (redir_fd[0] == -1)
	{
		perror(path);
		free(path);
		free(new);
		return (NULL);
	}
	free(path);
	return (new);
}

char	*get_exec(char *cmd, t_data *data)
{
	int	i;

	i = -1;
	while (data->paths[++i])
	{
		data->paths[i] = ft_strjoin(data->paths[i], cmd, 1);
		if (access(data->paths[i], F_OK | X_OK) == 0)
			return (data->paths[i]);
	}
	return (cmd);
}

void	go(char *cmd, t_data *data)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	path = get_exec(s_cmd[0], data);
	if (execve(path, s_cmd, data->env) <= -1)
	{
		errno = 3;
		perror(s_cmd[0]);
		ft_free_tab(data->cmd);
		ft_free_tab(s_cmd);
		ft_free_tab(data->paths);
		exit(0);
	}
}

void	exec(char *cmd, t_data *data, int *redir_fd)
{
	pid_t	pid;
	
	pid = fork();
	signals(2);
	if (pid == 0)
	{
		close(redir_fd[1]);
		dup2(redir_fd[0], 0);
		close(redir_fd[0]);
		close_n_dup(data);
		recoded(data, cmd);
		go(cmd, data);
	}
	else
	{
		close(redir_fd[0]);
		close(redir_fd[1]);
		close(data->p_fd[1]);
		dup2(data->p_fd[0], 0);
		close(data->p_fd[0]);
	}
}

int	still_in(char *cmd)
{
	int	i;
	
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '<')
			return (1);
		i++;
	}
	return (0);
}

int	still_out(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '>')
			return (1);
		i++;
	}
	return (0);
}

void	execution(t_data *data)
{
	int	i;
	int	redir_fd[2];
	
	i = -1;
	while (data->cmd[++i])
	{
		redir_fd[0] = dup(0);
		redir_fd[1] = dup(1);
		if (pipe(data->p_fd) == -1)
			exit(ft_dprintf(2, "\xE2\x9A\xA0\xEF\xB8\x8F Pipe error\n")); // faire une fonction pour exit proprement
		while (still_in(data->cmd[i]))
		{
			data->cmd[i] = redir_in(data, data->cmd[i], redir_fd);
			if (!data->cmd[i] || !*data->cmd[i])
				break;
		}
		if (!data->cmd[i] || !*data->cmd[i] || redir_fd[0] == -1)
		{
			close(data->p_fd[0]);
			close(data->p_fd[1]);
			close(redir_fd[1]);
			continue;
		}
		data->cmd[i] = ft_strtrim(data->cmd[i], " \t", 1);
		exec(data->cmd[i], data, redir_fd);
	}
}

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

void	edit_dollar(t_data *data, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
			data->dollar++;
		i++;
	}
	
}

int	main(int ac, char **av, char **env) 
{
	t_data	data;
	char *input;
	int	ret;
	(void)ac;
	(void)av;
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
		while (wait(NULL) > 0)
			;
		//if (data.child)
		if (!isatty(0))
			print(&data);
		ft_free_tab(data.cmd);
		dup2(data.base_fd[0], 0);
		close (data.base_fd[0]);
		dup2(data.base_fd[1], 1);
		close (data.base_fd[1]);
	}
	//ft_free_tab(data.paths);
	//ft_free_tab(data.env);
	return (0);
}
