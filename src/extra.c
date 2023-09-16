/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:43:07 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/17 00:03:50 by bvaujour         ###   ########.fr       */
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
    if (i >= 2)
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


int	cmd_print(void)
{
    char    *str;
    int     i;
	int		fd;

	fd = open("/tmp/minishell_save", O_RDONLY);
    i = 1;
    str = get_next_line(fd);
    if (!str)
        return (-1);
    while (str)
    {
        printf("(%d): %s", i, str);
        free(str);
        str = get_next_line(fd);
        i++;
    }
    ft_printf("select a command and press enter: ");
    str = get_next_line(0);
    if (!str)
        exit(0);
    i = ft_atoi(str);
	free(str);
	close (fd);
    return (i);
}

int	cmd_choice(t_data *data)
{
	int		fd;
	char	*str;
    int     i;

	if (ft_strlen(data->input) > 3)
		i = ft_atoi(data->input + 3) - 1;
	else
    	i = cmd_print() - 1;
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