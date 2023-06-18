/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 23:30:16 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/19 01:26:10 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*wildcards(t_data *data, char *cmd)
{
	DIR	*dir;
	struct dirent *entry;
	int	i;
	char	**spl;
	char	*new;
	(void)data;
	
	new = ft_strdup("");
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\t')
			cmd[i] = ' ';
		i++;
	}
	i = 0;
	spl = ft_split(cmd, ' ');
	while (spl[i])
	{
		//printf("spl[%d] = |%s|\n", i, spl[i]);
		if (spl[i][0] == '*' && !spl[i][1])
		{
			dir = opendir(".");
			entry = readdir(dir);
			while (entry != NULL)
			{
				if (entry->d_name[0] != '.')
				{
					//printf("%s\n", entry->d_name);
					new = ft_strjoin(new, ft_strdup(entry->d_name), 3);
					new = ft_strjoin(new, " ", 1);
				}
				entry = readdir(dir);
    		}
			closedir(dir);
		}
		else if (spl[i][0] == '*' && spl[i][1])
		{
			dir = opendir(".");
			entry = readdir(dir);
			while (entry != NULL)
			{
				int	j = ft_strlen(entry->d_name);
				if (entry->d_name[0] != '.')
				{
					while (j > 0 && entry->d_name[j] != spl[i][1])
						j--;
					if (j == 0)
					{
						entry = readdir(dir);
						continue ;
					}
					//printf ("entry->d_name + j = |%s|  spl[i] + 1 = |%s|\n", entry->d_name + j, spl[i] + 1);
					if (strcmp(entry->d_name + j, spl[i] + 1) == 0)
					{
						//printf("%s\n", entry->d_name);
						new = ft_strjoin(new, ft_strdup(entry->d_name), 3);
						new = ft_strjoin(new, " ", 1);
					}
				}
				entry = readdir(dir);
			}
			closedir(dir);
		}
		else
		{
			new = ft_strjoin(new, spl[i], 1);
			if (spl[i + 1])
				new = ft_strjoin(new, " ", 1);
		}
		i++;
	}
	//printf("new = |%s|\n", new);
	
	free(cmd);
	ft_free_tab(spl);
    return (new);
}