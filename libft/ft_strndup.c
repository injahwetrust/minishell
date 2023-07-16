/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 17:18:11 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/16 13:27:13 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*return a malloc string of the duplicated first n bytes of src if n >= 0
 return a malloc string with removed first n bytes of src if n < 0
 free src is opt == 1*/

static char *removendup(char *src, int n)
{
    char    *dup;
    int i;
    int len;

    i = 0;
    n = -n;
    len = ft_strlen(src);
    if (n > len)
        return (NULL);
    dup = malloc(sizeof(char) * (len - n) + 1);
    if (dup == NULL)
        return (NULL);
    while(src[n])
        dup[i++] = src[n++];
    dup[i] = '\0';
    return (dup);
}

char    *ft_strndup(char *src, int n, int opt)
{
    char    *dup;
    int i;
    int len;

    i = 0;
    if (!n)
        return (ft_strdup(""));
    if (n > 0)
    {
    	len = ft_strlen(src);
    	dup = malloc(sizeof(char) * n + 1);
    	if (!dup)
        	return (NULL);
    	while (i < n && i < len)
        {
            dup[i] = src[i];
            i++;
        }
        dup[i] = '\0';
    }
	else
		dup = removendup(src, n);
    if (opt == 1 && src)
        free(src);
    return(dup);
}

/*int	main(int ac, char **av)
{
    (void)ac;
	char	*str = "salut tout le monde";
	
	str = ft_strndup(str, atoi(av[1]), 0);
	ft_printf("%s\n", str);
	free(str);
}*/
