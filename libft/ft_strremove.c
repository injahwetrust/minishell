/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strremove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 15:10:38 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/11 23:32:34 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Remove the n'th occurence of to_del in src*/

char    *ft_strremove(char *src, char *to_del, int n, int opt)
{
    int i;
    int j;
    char    *new;
    
    if (!to_del || !*to_del || !src || !*src || n < 0 || n > ft_countoccur(src, to_del))
        return (src);
    new = malloc(sizeof(char) * (ft_strlen(src) - ft_strlen(to_del) + 1));
    if (!new)
        return (NULL);
    i = -1;
    j = ft_chrstrinstr(src, to_del, n);
    while (++i < j)
        new[i] = src[i];
    j += ft_strlen(to_del) - 1;
    while (src[++j])
        new[i++] = src[j];
    new[i] = '\0';
    if (opt == 1)
        free(src);
    return(new);
}
