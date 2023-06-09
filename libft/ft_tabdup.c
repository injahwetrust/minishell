/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 09:13:23 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/09 09:19:23 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    **ft_tabdup(char **tab)
{
    int     i;
    char    **new;
    
    i = 0;
    while (tab[i])
        i++;
    new = malloc(sizeof(char *) * (i + 1));
    if (!new)
        return(NULL);
    i = 0;
    while (tab[i])
    {
        new[i] = ft_strdup(tab[i]);
        i++;
    }
    new[i] = 0;
    return(new);
}