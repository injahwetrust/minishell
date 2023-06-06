/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 14:45:42 by bvaujour          #+#    #+#             */
/*   Updated: 2022/11/12 14:49:14 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcat(char *dest, char *src)
{
	int	size_dest;
	int	i;

	i = 0;
	size_dest = ft_strlen(dest);
	while (src[i] != '\0')
	{
		dest[size_dest + i] = src[i];
		i++;
	}
	dest[size_dest + i] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, char *s2, int option)
{
	char	*join;

	if (!s1 || !s2)
		return (0);
	join = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (join == 0)
		return (0);
	join[0] = '\0';
	ft_strcat(join, (char *)s1);
	ft_strcat(join, (char *)s2);
	if (option == 1)
		free(s1);
	else if (option == 2)
		free(s2);
	else if (option == 3)
	{
		free(s1);
		free(s2);
	}
	return (join);
}

/*int	main()
{
	char	*s1 = "hello";
	char	*s2 = "world";
	
	printf ("%s\n", ft_strjoin(s1, s2));
	printf ("%ld\n", ft_strlen(ft_strjoin(s1, s2)));
}*/
