/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:40:43 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/09 18:36:24 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{

	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
	write (fd, "\n", 1);
}

/*int	main()
{
	char	*str = "hello";

	ft_putendl_fd(str, 1);
	return (0);
}*/
