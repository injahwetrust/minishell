/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_countoccur.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 13:33:32 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/11 14:13:48 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*return the x time you found occurence of to_count in src*/

static int chr_in(char *src, char *to_count, int begin)
{
    int i;
    int j;

    i = -1;
    while (src[++i])
    {
        j = 0;
        while (src[i] == to_count[j])
        {
            i++;
            j++;
            if (!to_count[j])
                return (i - ft_strlen(to_count) + begin);
        }
        if (!src[i])
            break;
    }
    return (-1);
}

int ft_countoccur(char *src, char *to_count)
{
    int go_to;
    int i;
    int x;

    x = 0;
    i = 0;
    while ((go_to = chr_in(src + i, to_count, i)) != -1)
    {
        i = ft_strlen(to_count) + go_to;
        x++;
    }
    return (x);
}