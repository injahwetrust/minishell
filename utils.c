/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:11:04 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/14 01:05:30 by bvaujour         ###   ########.fr       */
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
	close(data->fd.base_fd[0]);
	close(data->fd.base_fd[1]);
	
	close(data->fd.p_fd[0]);
	dup2(data->fd.p_fd[1], 1);
	close(data->fd.p_fd[1]);
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

int		wrong_ident(t_data *data, char c)
{
	int	i;

	i = 0;
	while (data->wrong_char[i])
	{
		if (data->wrong_char[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*parse_export(t_data *data, char *input)
{
	int	i;
	int	equal;
	char	*new;
	
	i = 0;
	equal = 0;
	new = ft_strdup(input + 6);
	if (!new)
		return (NULL);	// faire une fonction pour exit proprement
	new = ft_strtrim(new, " \t", 1);
	if (!*new)
	{
		ft_dprintf(2, "Minishell: export: wrong identifier: ");
		return (NULL);
	}
	if (!new)
		return (NULL);	// faire une fonction pour exit proprement
	if (new[0] == '=')
		return (NULL);
	while (new[i] && new[i] != '=')
	{
		if (!in_ex(data, new[i]) || (i == 0 && (new[i] >= '0' && new[i] <= '9')))
		{
			i = 0;
			ft_dprintf(2, "Minishell: export: wrong identifier: ");
			while (new[i] != '=' && new[i] != ' ' &&  new[i] != '\t' && new[i])
				i++;
			write(2, new, i);
			ft_dprintf(2, "\n");
			return (NULL);
		}
		i++;
	}
	if (!new[i])
		return (NULL);
	i = 0;
	while (new[i])
	{
		if (new[i] == '=')
			equal = 1;
		else if (new[i] == ' ' && equal == 0)
			return (NULL);
		else if (new[i] == ' ' && equal == 1)
			new[i] = '\0';
		i++;
	}
	free(input);
	return (new);
}

void	print(t_data *data)
{
	int	ret;
	char	buff[50];
	
	(void)data;
	ret = 1;
	while (ret)
	{
		ret = read(0, buff, sizeof(buff));
		buff[ret] = '\0';
		ft_printf("%s", buff);
	}
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
void	free_all(t_data *data)
{
	rl_clear_history();
	free(data->prompt);
	free(data->input);
	ft_free_tab(data->paths);
	close (data->fd.base_fd[0]);
	close (data->fd.base_fd[1]);
	ft_free_tab(data->env);
	ft_printf("Exiting Minishell\n");
	signals(3);
}

int	still_in(char *cmd)
{
	int	i;
	
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '<')
			return (1);
		i++;
	}
	return (0);
}

int	still_out(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '>')
			return (1);
		i++;
	}
	return (0);
}

void	edit_dollar(t_data *data)
{
	int	i;

	i = 0;
	while (data->input[i])
	{
		if (data->input[i] == '$')
			data->dollar++;
		i++;
	}
	
}
