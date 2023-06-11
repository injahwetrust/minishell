/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chrstrinstr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 13:44:28 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/11 14:23:41 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* send a int to the first char of to_chr of the occur time to_chr was found in src
    return -1 in case of error*/

int ft_chrstrinstr(char *src, char *to_chr, int occur)
{
    int i;
    int j;

    i = 0;
    if (occur > ft_countoccur(src, to_chr) || occur <= 0|| !src || !to_chr)
        return (-1);
    while (occur)
    {
        j = 0;
        while (src[i] == to_chr[j])
        {
            i++;
            j++;
            if (!to_chr[j])
               occur--;
        }
        if (!src[i])
            break;
        i++;
    }
    return (i - ft_strlen(to_chr));
}

/*int main()
{
    char    *src = "apr je aprenapr apres";
    char    *to_chr = "apr";
    
    int removed = ft_chrstrinstr(src, to_chr, 4);
    printf("removed = %d\n", removed);
}*/
