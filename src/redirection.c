/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 09:15:25 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/15 19:07:37 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	read_in(char *path, int fd)
{
	char	*ret;

	signals(3);
	while (1)
	{
		ft_dprintf(2, BO_GREEN"(%s)input:"RESET, path);
		ret = get_next_line(0);
		if (ret == NULL)
			ft_dprintf(2, "\n"MINI H_ERROR, path);
		if (!ret || (ft_strncmp(ret, path, ft_strlen(path)) == 0
				&& ft_strlen(path) == ft_strlen(ret) - 1))
		{
			get_next_line(fd);
			free(ret);
			return (0);
		}
		write(1, ret, ft_strlen(ret));
		free(ret);
	}
	return (0);
}

static void	heredoc(t_data *data, char *path)
{
	int	pid;
	int	ret;
	int	fd;

	signals(4);
	if (pipe(data->fd.p_fd) == -1)
		end(data);
	pid = fork();
	if (pid == 0)
	{
		fd = open("/tmp/free_gnl", O_RDONLY | O_CREAT, 0644);
		redirect_close(data);
		ret = read_in(path, fd);
		(close(fd), step0(data), exit(ret));
	}
	g_last_ret = pid;
	waitpid(pid, NULL, 0);
	if (close(data->fd.p_fd[1]) == -1)
		(perror("Minishell"), end(data));
	if (dup2(data->fd.p_fd[0], 0) == -1)
		(perror("Minishell"), end(data));
	if (close(data->fd.p_fd[0]) == -1)
		(perror("Minishell"), end(data));
}

static int	edit_in(t_data *data, char *in)
{
	int	i;
	int	doc;
	int	fd;

	i = 0;
	while (in[i] == '<')
		i++;
	doc = i;
	while (in[i] == ' ')
		i++;
	if (doc == 2)
		heredoc(data, in + i);
	else
	{
		fd = open(in + i, O_RDONLY);
		if (fd == -1)
		{
			ft_dprintf(2, "Minishell: %s: %s\n", in + i, strerror(errno));
			return (1);
		}
		dup2(fd, 0);
		close(fd);
	}
	return (0);
}

static int	edit_out(char *out)
{
	int	i;
	int	fd;
	int	append;

	i = 0;
	while (out[i] == '>')
		i++;
	append = i;
	while (out[i] == ' ')
		i++;
	if (append == 2)
		fd = open(out + i, O_CREAT | O_APPEND | O_WRONLY, 0644);
	else
		fd = open(out + i, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
	{
		ft_dprintf(2, "Minishell: %s: %s\n", out + i, strerror(errno));
		return (1);
	}
	dup2(fd, 1);
	close (fd);
	return (0);
}

int	redirection(t_data *data, t_cmd *ccmd)
{
	int		i;

	i = -1;
	while (ccmd->in[++i])
	{
		if (ft_strcmp(ccmd->in[i], "<") == 0)
			continue ;
		if (edit_in(data, ccmd->in[i]))
		{
			g_last_ret = 1;
			return (1);
		}
	}
	i = -1;
	while (ccmd->out[++i])
	{
		if (ft_strcmp(ccmd->out[i], ">") == 0)
			continue ;
		if (edit_out(ccmd->out[i]))
		{
			g_last_ret = 1;
			return (1);
		}
	}
	return (0);
}
