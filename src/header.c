/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mablatie <mablatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 22:20:41 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/22 12:17:09 by mablatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static void	marged_header(struct winsize win)
{
	int	i;
	int	marge;

	marge = ft_strlen(HEADER1) / 2;
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	printf(C_GREEN HEADER1);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	printf(HEADER2);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	printf(HEADER3);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	printf(HEADER4);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	printf(HEADER5 RESET GRAY HEADER6 RESET"\n\n");
}

void	header(void)
{
	struct winsize	win;

	if (!ioctl(1, TIOCGWINSZ, &win))
		marged_header(win);
	else
		ft_printf(C_GREEN HEADER1 HEADER2 HEADER3 HEADER4 HEADER5 RESET);
	write(1, "\n\n", 2);
}
