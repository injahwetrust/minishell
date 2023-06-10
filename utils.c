/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vanitas <vanitas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:11:04 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/10 16:33:47 by vanitas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	end_process(t_data *data)
{
	ft_free_tab(data->cmd);
	ft_free_tab(data->paths);
	exit(0);
}

void	ft_free_tab(char **tab)
{
	int	i;
	
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

void	close_n_dup(t_data *data)
{
	close(data->base_fd[0]);
	close(data->base_fd[1]);
	close(data->p_fd[0]);
	dup2(data->p_fd[1], 1);
	close(data->p_fd[1]);
}

char	*parse_unset(char *input)
{
	int	i;
	char	*new;

	i = 5;
	while ((input[i] == ' ' || input[i] == '\t') && input[i])
		i++;
	new = ft_strdup(input + i);
	if (new == NULL)
		exit(0); // faire une fonction pour exit proprement
	new = ft_strtrim(new, " \t", 1);
	if (new == NULL)
		exit(0); // faire une fonction pour exit proprement
	free(input);
	return (new);
}

char	*parse_export(char *input)
{
	int	i;
	int	equal;
	char	*new;
	
	i = 0;
	equal = 0;
	new = ft_strdup(input + 6);
	if (!new)
		return (NULL);
	new = ft_strtrim(new, " \t", 1);
	if (!new)
		return (NULL);
	if (new[0] == '=')
		return (NULL);
	while (new[i] && new[i] != '=')
		i++;
	if (!new[i])
		return (NULL);
	i = 0;
	while (new[i])
	{
		if (new[i] == '=')
			equal = 1;
		if (new[i] == ' ' && equal == 0)
		{
			ft_dprintf(2, "minishell: export: wrong identifier\n");
			return (NULL);
		}
		if (new[i] == ' ' && equal == 1)
			new[i] = '\0';
		i++;
	}
	free(input);
	return (new);
}

void	edit_prompt(t_data *data, char *cwd)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	while (cwd[i])
		i++;
	while (cwd[i] != '/' && i > 0)
		i--;
	while (j < i)
	{
		cwd++;
		j++;
	}
	data->prompt = ft_strjoin(BG_GREEN BO_BLACK"Minishell~", getenv("USER"), 0);
	data->prompt = ft_strjoin(data->prompt, RESET BO_GREEN"🐸", 1);
	data->prompt = ft_strjoin(data->prompt, cwd, 1);
	data->prompt = ft_strjoin(data->prompt,RESET"$ ", 1);
}

void	edit_paths(t_data *data)
{
    int     i;
    
    i = -1;
    data->paths = ft_split(getenv("PATH"), ':');
    while (data->paths[++i])
		data->paths[i] = ft_strjoin(data->paths[i], "/", 1);
}
void	free_all(t_data *data, char *input)
{
	rl_clear_history();
	free(data->prompt);
	free(input);
	ft_free_tab(data->paths);
	close (data->base_fd[0]);
	close (data->base_fd[1]);
	ft_free_tab(data->env);
	ft_printf("Exiting Minishell\n");
	signals(3);
}