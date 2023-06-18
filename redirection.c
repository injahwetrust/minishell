/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:09:46 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/18 15:00:26 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char *cmd, int begin)
{
	int	i;
	char	*file;

	i = 1 + begin;
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

char	*get_in(char *cmd, int begin)
{
	int	i;
	char	*file;

	i = 1 + begin;
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
		i++;
	if (cmd[i] == '>' || cmd[i] == '<')
	{
		ft_dprintf(2, "syntax error near unexpected symbol « %c »\n", cmd[i]);
		return (ft_strdup("JOHNCARPENTER&DONALDDUCK"));
	}
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '<' && cmd[i] != '>')
		i++;
	file = ft_strndup(cmd, i, 0);
	if (!file)
		exit(0); // faire une fonction pour exit proprement
	return (file);
}

void	heredoc(t_data *data, char *path)
{
	int	pid;
	char	*ret;
	int	p_fd[2];
	
	if (pipe(p_fd) == -1)
		exit(ft_dprintf(2, "\xE2\x9A\xA0\xEF\xB8\x8F Pipe error\n")); // faire une fonction pour exit proprement
	pid = fork();
	signals(data, 4);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(data->fd.redir_fd[0]);
		close(data->fd.redir_fd[1]);
		close(data->fd.p_fd[0]);
		close(data->fd.p_fd[1]);
		close(data->fd.base_fd[1]);
		dup2(data->fd.base_fd[0], 0);
		close(data->fd.base_fd[0]);
		close(p_fd[0]);
		dup2(p_fd[1], 1);
		close(p_fd[1]);
		// ft_free_tab(data->paths);
		// ft_free_tab(data->env);
		// ft_free_tab(data->cmd);
		while (1)
		{
			ft_dprintf(2, BO_GREEN"(%s)input:"RESET, path);
			ret = get_next_line(0);
			if (ft_strncmp(ret, path, ft_strlen(path)) == 0
				&& ft_strlen(path) == strlen(ret) - 1)
			{
				get_next_line(-99);
				free(ret);
				exit(0);
			}
				write(1, ret, ft_strlen(ret));
				free(ret);
		}
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(p_fd[1]);
		dup2(p_fd[0], 0);
		close(p_fd[0]);
	}
}

char	*redir_in(t_data *data, char *cmd)
{
	int	i;
	char	*untrim;
	char	*path;
	char	*new;
	(void)data;
	
	i = 0;
	while (cmd[i] && cmd[i] != '<')
		i++;
	if (cmd[i + 1] == '<')
		data->fd.heredoc = 1;
	else
		data->fd.heredoc = 0;
	untrim = get_in(cmd + i, data->fd.heredoc);
	if (untrim == NULL)
		free_all(data);
	if ((ft_strcmp(untrim, "JOHNCARPENTER&DONALDDUCK") == 0))
	{
		free (cmd);
		free(untrim);
		return (ft_strdup("JOHNCARPENTER&DONALDDUCK"));
	}
	path = get_path(untrim, data->fd.heredoc);
	if (path == NULL)
		free_all(data);
	close(data->fd.redir_fd[0]);
	if (data->fd.heredoc)
		heredoc(data, path);
	else
	{
		dprintf(2, "entrée redirigée depuis %s\n", path);
		data->fd.redir_fd[0] = open(path, O_RDONLY, 0644);
	}
	new = ft_strremove(cmd, untrim, 1, 0);
	if (!new)
		free_all(data);
	free(untrim);
	free(cmd);
	if (data->fd.redir_fd[0] == -1)
	{
		perror(path);
		free(path);
		free(new);
		return (NULL);
	}
	free(path);
	return (new);
}

char	*get_out(char *cmd, int begin)
{
	int	i;
	char	*file;

	i = 1 + begin;
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
		i++;
	if (cmd[i] == '>' || cmd[i] == '<')
	{
		ft_dprintf(2, "syntax error near unexpected token « %c »\n", cmd[i]);
		return (ft_strdup("JOHNCARPENTER&DONALDDUCK"));
	}
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '>' && cmd[i] != '<')
		i++;
	file = ft_strndup(cmd, i, 0);
	if (!file)
		exit(0); // faire une fonction pour exit proprement
	return (file);
}

char	*redir_out(t_data *data, char *cmd)
{
	int	i;
	char	*untrim;
	char	*path;
	char	*new;
	(void)data;
	
	i = 0;
	while (cmd[i] && cmd[i] != '>')
		i++;
	if (cmd[i + 1] == '>')
		data->fd.append = 1;
	untrim = get_out(cmd + i, data->fd.append);
	if (untrim == NULL)
		free_all(data);
	if ((ft_strcmp(untrim, "JOHNCARPENTER&DONALDDUCK") == 0))
	{
		free (cmd);
		free(untrim);
		return (ft_strdup("JOHNCARPENTER&DONALDDUCK"));
	}
	path = get_path(untrim, data->fd.append);
	close(data->fd.redir_fd[1]);
	if (data->fd.append)
		data->fd.redir_fd[1] = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
	{
		data->fd.redir_fd[1] = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		ft_dprintf(2, "sortie redirigée vers %s\n", path);
	}
	new = ft_strremove(cmd, untrim, 1, 0);
	if (!new)
		free_all(data);
	free(cmd);
	free(untrim);
	if (data->fd.redir_fd[1] == -1)
	{
		perror(path);
		free(path);
		free(new);
		return (NULL);
	}
	free(path);
	return (new);
}