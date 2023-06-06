/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 15:13:26 by bvaujour          #+#    #+#             */
/*   Updated: 2022/11/12 15:17:05 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	in_cs(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	*strim;

	if (!s1 || !set)
		return (0);
	i = 0;
	j = ft_strlen(s1);
	k = 0;
	while (in_cs(s1[i], set))
		i++;
	while (in_cs(s1[j - 1], set) == 1)
		j--;
	if (j == 0)
		return (ft_strdup(""));
	strim = malloc (sizeof(char) * (j - i + 1));
	if (strim == 0)
		return (0);
	while (i < j)
		strim[k++] = s1[i++];
	strim[k] = '\0';
	return ((char *)strim);
}

/*int	main()
{
	char	*str = "123456789";
	//char	*str2 = "tripouille";
	//char	*set = "123";
	str = str + 2;
	//printf ("%s\n", ft_strtrim(str, set));
	//printf ("%ld\n", strlen(str2));
	printf ("%s\n", str);
}*/
