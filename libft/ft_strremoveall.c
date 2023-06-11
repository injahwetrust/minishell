/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strremoveall.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 11:28:18 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/11 15:27:01 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*remove all occurences of to_del in src, option = 1 to free src, option = 2 to free to_del, option = 3 to free both*/

static int del_in(char *src, char *to_del, int begin)
{
    int i;
    int j;

    i = -1;
    while (src[++i])
    {
        j = 0;
        while (src[i] == to_del[j])
        {
            i++;
            j++;
            if (!to_del[j])
                return (i - ft_strlen(to_del) + begin);
        }
        if (!src[i])
            break;
    }
    return (-1);
}
 
static int x_str(char *src, char *to_del)
{
    int go_to;
    int i;
    int x;

    x = 0;
    i = 0;
    while ((go_to = del_in(src + i, to_del, i)) != -1)
    {
        i = ft_strlen(to_del) + go_to;
        x++;
    }
    return (x);
}

char    *ft_strremoveall(char *src, char *to_del, int opt)
{
    int i;
    int j;
    int go_to;
    char    *new;
    
    i = 0;
    j = 0;
    if (!to_del || !*to_del || !src || !*src || del_in(src, to_del, i) == -1)
        return (src);
    new = malloc(sizeof(char) * (ft_strlen(src) - (ft_strlen(to_del) * x_str(src, to_del)) + 1));
    if (!new)
        return (NULL);
    while ((go_to = del_in(src + i, to_del, i)) != -1)
    {
         while (i < go_to)
            new[j++] = src[i++];
        i += ft_strlen(to_del);
    }
    while (src[i])
        new[j++] = src[i++];
    new[j] = '\0';
    if (opt == 1)
        free(src);
    return (new);
}

/*int main()
{
    char    *src = "apr je aprenapr apres";
    char    *to_del = "apr";
    
    char    *removed = ft_strremoveall(src, to_del, 0);
    printf("removed = %s", removed);
    free(removed);
}*/