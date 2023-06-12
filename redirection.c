/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 11:09:46 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/12 12:59:21 by bvaujour         ###   ########.fr       */
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

char	*get_out(char *cmd)
{
	int	i;
	char	*file;

	i = 1;
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
		i++;
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '>')
		i++;
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
	untrim = get_out(cmd + i);
	ft_dprintf(2, "untrim = %s\n", untrim);
	path = get_path(untrim);
	ft_dprintf(2, "path = %s\n", path);
	close(redir_fd[1]);
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