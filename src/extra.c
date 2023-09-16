/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 17:43:07 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/16 22:47:28 by bvaujour         ###   ########.fr       */
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


char	*cmd_core(int fd)
{
    char    *str;
    int     i;

    i = 1;
    str = get_next_line(fd);
    if (!str)
        exit(0);
    write(1, "\n", 1);
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
    if (i)
    {
        close(fd);
        fd = open("/tmp/minishell_save", O_RDONLY);
        while (i)
        {
            free(str);
            str = get_next_line(fd);
            i--;
        }
    }
    close(fd);
    return (str);
}

void	cmd_choice(int sig)
{
	int		fd;
	char	*str;

	fd = open("/tmp/minishell_save", O_RDONLY);
	if (fd == -1)
		printf("\nno saved commands\n");
	else
		str = cmd_core(fd);
	if (str)
	{
		str = ft_strtrim(str, "\n", 1);
		rl_replace_line(str, 0);
		free(str);
	}
	else
		rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
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