/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 09:15:25 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/12 15:01:14 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	read_in(char *path)
{
	char	*ret;
	char	*limiter;
	int		fd;
	int		i;

	signals(3);
	fd = open("/tmp/mini_here_doc", O_CREAT | O_WRONLY, 0644);
	limiter = ft_strjoin(path, ": ", 0);
	while (1)
	{
		i = 0;
		ret = readline(limiter);
		if (ret == NULL)
		{
			free(limiter);
			close(fd);
			ft_dprintf(2, MINI H_ERROR, path);
			return (1);
		}
		if (ft_strcmp(ret, path) == 0)
		{
			dprintf(2, "test\n");
			free(ret);
			free(limiter);
			close(fd);
			break ;
		}
		while (ret[i])
		{
			write(fd, &ret[i], 1);
			i++;
		}
		write(fd, "\n", 1);
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
	pid = fork();
	if (pid == 0)
	{
		dup2(data->fd.base_fd[0], 0);
		ret = read_in(path);
		step0(data);
		exit(ret);
	}
	g_last_ret = pid;
	waitpid(pid, NULL, 0);
	fd = open("/tmp/mini_here_doc", O_RDONLY);
	if (fd == -1)
		end(data);
	dup2(fd, 0);
	close(fd);
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
