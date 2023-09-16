/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 00:29:58 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/16 21:48:42 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	check_ghost(t_data *data, char *str)
{
	int		i;
	char	*part;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	part = ft_strndup(str, i, 0);
	if (is_in_env(data, part))
		remove_from_ghost(data, part);
	free(part);
}

void	toggle_signals_on(void)
{
	struct termios		terminal;

	tcgetattr(1, &terminal);
	terminal.c_lflag &= ~ECHOCTL;
	tcsetattr(1, TCSANOW, &terminal);
}

void	toggle_signals_off(void)
{
	struct termios		terminal;

	tcgetattr(1, &terminal);
	terminal.c_lflag |= ECHOCTL;
	tcsetattr(1, TCSANOW, &terminal);
}

int	print_history(void)
{
	char	*str;
	int		fd;
	int		i;

	i = 1;
	fd = open("/tmp/minishell_history", O_RDONLY);
	str = get_next_line(fd);
	while (str)
	{
		printf("  %d", i);
		printf("  %s", str);
		free(str);
		str = get_next_line(fd);
		i++;
	}
	close (fd);
	return (0);
}

void	history(t_data *data)
{
	int			fd;

	fd = open("/tmp/minishell_history", O_RDWR | O_CREAT | O_APPEND, 0644);
	add_history(data->input);
	ft_dprintf(fd, "%s\n", data->input);
	close(fd);
}
