/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 13:08:17 by bvaujour          #+#    #+#             */
/*   Updated: 2022/09/22 13:51:17 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	i;
	unsigned int	size_src;

	i = 0;
	size_src = 0;
	while (src[size_src] != '\0')
		size_src++;
	if (size == 0)
		return (size_src);
	while (src[i] != '\0' && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (size_src);
}

/*int	main()
{
	char	src[25] = "je suis une loutre";
	char	dest[50];

	printf("%ld\n", ft_strlcpy(dest, src, 35));
	printf("%s", dest);
}*/
