/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:43:07 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/17 01:53:51 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

int    save_cmd(t_data *data, char **s_cmd)
{
    int i;
    int fd;

    i = 0;
    while (s_cmd[i])
        i++;
    if (i > 2)
    {
        printf("Minishell: save: too many arguments\n");
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

void	new_save(char **s_cmd, char **save)
{
	int	fd;
	int	i;
	char	*str;

	i = 0;
	fd = open("/tmp/minishell_save", O_RDONLY);
	str = get_next_line(fd);
	while (str)
	{
		save[i] = ft_strdup(str);
		free(str);
		str = get_next_line(fd);
		i++;
	}
	save[i] = 0;
	(close(fd), unlink("/tmp/minishell_save"));
	fd = open("/tmp/minishell_save", O_WRONLY | O_APPEND | O_CREAT, 0644);
	i = 0;
	while (save[i])
	{
		str = get_next_line(fd);
		if (i != ft_atoi(s_cmd[1]) - 1)
			ft_dprintf(fd, "%s", save[i]);
		free(str);
		i++;
	}
	close(fd);
}

int	delete_cmd(t_data *data, char **s_cmd)
{
	int		fd;
	char	*str;
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
	fd = open("/tmp/minishell_save", O_RDONLY);
	if (fd == -1)
	{
		ft_dprintf(2, "Minishell: cmd: no command saved\n");
		return (1);
	}
	str = get_next_line(fd);
	while (str)
	{
		free(str);
		str = get_next_line(fd);
		i++;
	}
	save = malloc(sizeof(char *) * (i + 1));
	if (!save)
		end(data);
	close(fd);
	new_save(s_cmd, save);
	ft_free_tab(save);
	return (0);
}

void	cmd_print(void)
{
    char    *str;
    int     i;
	int		fd;

	printf("cmd Saved commands\n\n");
	fd = open("/tmp/minishell_save", O_RDONLY);
    i = 1;
    str = get_next_line(fd);
    if (!str)
	{
		close (fd);
		ft_dprintf(2, "Minishell: cmd: no command saved\n");
		return ;
	}
    while (str)
    {
        printf("(%d): %s", i, str);
        free(str);
        str = get_next_line(fd);
        i++;
    }
	close(fd);
}

int	cmd_choice(t_data *data)
{
	int		fd;
	char	*str;
    int     i;

	if (ft_strlen(data->input) > 3)
		i = ft_atoi(data->input + 3) - 1;
	else
	{
		cmd_print();
		printf("\nRerun with index to execute associated command\n");
		printf("Run rmcmd to delete command\n");
		return (1);
	}
	if (i <= -1)
		return (1);
	fd = open("/tmp/minishell_save", O_RDONLY);
	if (fd == -1)
		return (ft_dprintf(2, "no saved commands\n"), 1);
    str = get_next_line(fd);
    while (str)
    {
        if (i == 0)
            break ;
        free(str);
        str = get_next_line(fd);
        i--;
    }
	if (!str)
	{
		close(fd);
		return (1);
	}
	free(data->input);
	data->input = ft_strtrim(str, "\n", 1);
	close(fd);
    return (0);
}

void    boot_history(void)
{
    char    *str;
    int     fd;

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