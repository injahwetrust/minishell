/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 00:29:58 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/15 17:27:10 by mablatie         ###   ########.fr       */
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

int	print_history(t_data *data, char **s_cmd)
{
	char	*str;
	int		fd;

	(void)data;
	fd = open("/tmp/minishell_history", O_RDONLY);
	(void)s_cmd;
	str = get_next_line(fd);
	while (str)
	{
		printf("%s", str);
		free(str);
		str = get_next_line(fd);
	}
	close (fd);
	return (0);
}

void	history(t_data *data)
{
	int			fd;
	char		*str;
	static int	index = 0;

	fd = open("/tmp/minishell_history", O_RDWR | O_CREAT | O_APPEND, 0644);
	if (index == 0)
	{
		str = get_next_line(fd);
		while (str)
		{
			index++;
			free(str);
			str = get_next_line(fd);
		}
	}
	add_history(data->input);
	ft_dprintf(fd, "%d ", index);
	ft_dprintf(fd, "%s\n", data->input);
	close(fd);
	index++;
}
