/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 00:29:58 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/15 12:00:23 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

void	check_ghost(t_data *data, char *str)
{
	int		i;
	char	*part;
	(void)data;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	part = ft_strndup(str, i, 0);
	if(is_in_env(data, part))
		remove_from_ghost(data, part);
	free(part);
}

void	toggleSignalsOn(void)
{
	struct termios		terminal;
 
	tcgetattr(1, &terminal);
	terminal.c_lflag &= ~ECHOCTL;
	tcsetattr(1, TCSANOW, &terminal);
}

void	toggleSignalsOff(void)
{
	struct termios		terminal;
 
	tcgetattr(1, &terminal);
	terminal.c_lflag |= ECHOCTL;
	tcsetattr(1, TCSANOW, &terminal);
}

int	print_history(t_data *data, char **s_cmd)
{
	
}
