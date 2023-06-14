/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:09:46 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/14 02:34:35 by bvaujour         ###   ########.fr       */
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
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '<' && cmd[i] != '>')
		i++;
	if (cmd[i] == '<')
	{
		ft_dprintf(2, "syntax error near unexpected symbol « %c »\n", cmd[i]);
		return (ft_strdup("JOHNCARPENTER&DONALDDUCK"));
	}
	file = ft_strndup(cmd, i, 0);
	if (!file)
		exit(0); // faire une fonction pour exit proprement
	return (file);
}

void	heredoc(t_data *data, char *path)
{
	char	*ret;
	data->fd.redir_fd[0] = open("heredoc", O_CREAT | O_TRUNC | O_RDWR, 0644);
	while (1)
	{
		ret = get_next_line(0);
		if (ft_strncmp(ret, path, ft_strlen(path)) == 0
			&& ft_strlen(path) == strlen(ret) - 1)
		{
			get_next_line(-99);
			close (data->fd.redir_fd[0]);
			data->fd.redir_fd[0] = open("heredoc", O_CREAT | O_APPEND | O_RDWR, 0644);
			free(ret);
			return ;
		}
			write(data->fd.redir_fd[0], ret, ft_strlen(ret));
			free(ret);
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
		data->fd.redir_fd[0] = open(path, O_RDONLY, 0644);
	new = ft_strremove(cmd, untrim, 1, 0);
	if (new)
		free_all(data);
	free(untrim);
	free(cmd);
	if (data->fd.redir_fd[0] == -1)
	{
		perror(path);
		free(path);
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
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '>' && cmd[i] != '<')
		i++;
	if (cmd[i] == '>')
	{
		ft_dprintf(2, "syntax error near unexpected symbol « %c »\n", cmd[i]);
		return (ft_strdup("JOHNCARPENTER&DONALDDUCK"));
	}
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
		data->fd.redir_fd[1] = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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