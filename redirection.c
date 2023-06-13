/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:09:46 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/13 10:09:03 by bvaujour         ###   ########.fr       */
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

void	heredoc(char *path, int *redir_fd)
{
	char	*ret;
	redir_fd[0] = open("heredoc", O_CREAT | O_TRUNC | O_RDWR, 0644);
	while (1)
	{
		ret = get_next_line(0);
		if (ft_strncmp(ret, path, ft_strlen(path)) == 0
			&& ft_strlen(path) == strlen(ret) - 1)
		{
			get_next_line(-99);
			close (redir_fd[0]);
			redir_fd[0] = open("heredoc", O_CREAT | O_APPEND | O_RDWR, 0644);
			free(ret);
			return ;
		}
			write(redir_fd[0], ret, ft_strlen(ret));
			free(ret);
		}
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
	if (cmd[i + 1] == '<')
	{
		data->heredoc = 1;
		untrim = get_in(cmd + i, data->heredoc);
	}
	else
	{
		data->heredoc = 0;
		untrim = get_in(cmd + i, data->heredoc);
	}
	if ((ft_strcmp(untrim, "JOHNCARPENTER&DONALDDUCK") == 0))
	{
		free (cmd);
		free(untrim);
		return (ft_strdup("JOHNCARPENTER&DONALDDUCK"));
	}
	ft_dprintf(2, "untrim = %s\n", untrim);
	path = get_path(untrim, data->heredoc);
	ft_dprintf(2, "path = %s\n", path);
	close(redir_fd[0]);
	if (data->heredoc)
		heredoc(path, redir_fd);
	else
		redir_fd[0] = open(path, O_RDONLY, 0644);
	new = ft_strremove(cmd, untrim, 1, 1);
	ft_dprintf(2, "new= %s\n", new);
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

char	*redir_out(t_data *data, char *cmd, int *redir_fd)
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
		data->append = 1;
	untrim = get_out(cmd + i, data->append);
	if ((ft_strcmp(untrim, "JOHNCARPENTER&DONALDDUCK") == 0))
	{
		free (cmd);
		free(untrim);
		return (ft_strdup("JOHNCARPENTER&DONALDDUCK"));
	}
	ft_dprintf(2, "untrim = %s\n", untrim);
	path = get_path(untrim, data->append);
	ft_dprintf(2, "path = %s\n", path);
	close(redir_fd[1]);
	if (data->append)
		redir_fd[1] = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		redir_fd[1] = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	new = ft_strremove(cmd, untrim, 1, 1);
	free(untrim);
	if (redir_fd[1] == -1)
	{
		perror(path);
		free(path);
		free(new);
		return (NULL);
	}
	free(path);
	return (new);
}