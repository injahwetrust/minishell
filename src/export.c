/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 12:09:08 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/15 00:44:12 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static void	print_classed_declare(char **dec)
{
	int	i;
	int	j;
	int	token;

	i = -1;
	while (dec[++i])
	{
		j = -1;
		token = 1;
		write(1, "declare -x ", 11);
		while (dec[i][++j])
		{
			write(1, &dec[i][j], 1);
			if (dec[i][j] == '=' && token == 1)
			{
				write(1, "\"", 1);
				token = 0;
			}
		}
		if (token == 0)
			write(1, "\"", 1);
		write(1, "\n", 2);
	}
}

static void	class_alpha(char **dec)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (dec[++i])
	{
		j = 0;
		while (dec[i - 1][j] == dec[i][j])
			j++;
		if (dec[i - 1][j] > dec[i][j])
		{
			tmp = dec[i - 1];
			dec[i - 1] = dec[i];
			dec[i] = tmp;
			i = 0;
		}
	}
	print_classed_declare(dec);
}

int	print_declare(t_data *data)
{
	char	**dec;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (data->env[i])
		i++;
	while (data->ghost[j])
		j++;
	dec = malloc(sizeof(char *) * (i + j + 1));
	if (!dec)
		end(data);
	i = -1;
	j = -1;
	while (data->env[++i])
		dec[i] = data->env[i];
	while (data->ghost[++j])
		dec[i + j] = data->ghost[j];
	dec[i + j] = data->ghost[j];
	class_alpha(dec);
	free(dec);
	return (0);
}
