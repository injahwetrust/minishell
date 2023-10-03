/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:43:07 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/20 13:31:11 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

int	save_cmd(t_data *data, char **s_cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (s_cmd[i])
		i++;
	if (i > 2)
	{
		printf("Minishell: save: can save only one argument,");
		printf(" no argument will save last command\n");
		return (1);
	}
	fd = open("/tmp/minishell_save", O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd == -1)
		return (1);
	if (i == 1)
		ft_dprintf(fd, "%s\n", data->last_cmd);
	else
		ft_dprintf(fd, "%s\n", s_cmd[1]);
	close(fd);
	return (0);
}

void	cmd_print(void)
{
	char	*str;
	int		i;
	int		fd;

	printf("\e[33;3mcmd is a tool to use quickly commands\n\n");
	printf("Saved commands:\n\n");
	fd = open("/tmp/minishell_save", O_RDONLY);
	i = 1;
	str = get_next_line(fd);
	if (!str)
	{
		if (fd != -1)
			close (fd);
		ft_dprintf(2, "Minishell: cmd: no command saved\n");
		return ;
	}
	while (str)
	{
		printf("  %d):  %s", i, str);
		free(str);
		str = get_next_line(fd);
		i++;
	}
	close(fd);
}

int	cmd_replace(t_data *data, int fd, int i)
{
	char	*str;

	str = get_next_line(fd);
	while (str)
	{
		if (i == 0)
			break ;
		free(str);
		str = get_next_line(fd);
		i--;
	}
	close(fd);
	if (!str)
		return (1);
	ft_printf("\e[33;3m\e[33;2;37m%s\e[0m", str);
	free(data->input);
	data->input = ft_strtrim(str, "\n", 1);
	return (0);
}

int	cmd_choice(t_data *data)
{
	int		fd;
	int		i;

	if (ft_strlen(data->input) > 3)
		i = ft_atoi(data->input + 3) - 1;
	else
	{
		cmd_print();
		printf("\nRerun with index to execute associated command\n");
		printf("Run save to add last command\n");
		printf("Run rmcmd to delete command\n");
		return (1);
	}
	if (i <= -1)
		return (1);
	fd = open("/tmp/minishell_save", O_RDONLY);
	if (fd == -1)
		return (ft_dprintf(2, "Minishell: cmd: no command saved\n"), 1);
	return (cmd_replace(data, fd, i));
}

void	boot_history(void)
{
	char	*str;
	int		fd;

	fd = open("/tmp/minishell_history", O_RDONLY);
	if (fd == -1)
		return ;
	str = get_next_line(fd);
	if (!str)
		return ;
	while (str)
	{
		str = ft_strtrim(str, "\n", 1);
		if (!str)
		{
			close(fd);
			return ;
		}
		add_history(str);
		free(str);
		str = get_next_line(fd);
	}
	close(fd);
}
