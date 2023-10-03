/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 09:30:34 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/20 12:14:14 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static void	new_save(char **s_cmd, char **save)
{
	int		fd;
	int		i;
	char	*str;

	i = 0;
	fd = open("/tmp/minishell_save", O_RDONLY);
	str = get_next_line(fd);
	while (str)
	{
		save[i++] = ft_strdup(str);
		free(str);
		str = get_next_line(fd);
	}
	save[i] = 0;
	(close(fd), unlink("/tmp/minishell_save"));
	fd = open("/tmp/minishell_save", O_WRONLY | O_APPEND | O_CREAT, 0644);
	i = -1;
	while (save[++i])
	{
		str = get_next_line(fd);
		if (i != ft_atoi(s_cmd[1]) - 1)
			ft_dprintf(fd, "%s", save[i]);
		free(str);
	}
	close(fd);
}

static int	count_cmd(void)
{
	int		i;
	int		fd;
	char	*str;

	i = 0;
	fd = open("/tmp/minishell_save", O_RDONLY);
	if (fd == -1)
	{
		ft_dprintf(2, "Minishell: cmd: no command saved\n");
		return (-1);
	}
	str = get_next_line(fd);
	while (str)
	{
		free(str);
		str = get_next_line(fd);
		i++;
	}
	close(fd);
	return (i);
}

int	delete_cmd(t_data *data, char **s_cmd)
{
	char	**save;
	int		i;

	i = 0;
	if (!s_cmd[1])
	{
		cmd_print();
		printf("\nRerun with index to delete associated command\n");
		printf("Rerun with -all to delete all commands\n");
		return (0);
	}
	if (ft_strcmp(s_cmd[1], "-all") == 0)
	{
		unlink("/tmp/minishell_save");
		return (0);
	}
	i = count_cmd();
	if (i == -1)
		return (1);
	save = malloc(sizeof(char *) * (i + 1));
	if (!save)
		end(data);
	new_save(s_cmd, save);
	ft_free_tab(save);
	return (0);
}
