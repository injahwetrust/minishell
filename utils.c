/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:11:04 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/19 13:06:41 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	end(t_data *data, char *ret)
{
	(void)data;
	ft_printf("exit\n");
	if (ft_str1_ishigher(ret, "9223372036854775807") == 1)
	{
		ft_dprintf(2, "Minishell: exit: %s: numeric argument required\n", ret);
		exit(2);
	}
	if (ft_str1_ishigher(ret, "-9223372036854775809") <= 0)
	{
		ft_dprintf(2, "Minishell: exit: %s: numeric argument required\n", ret);
		exit(2);
	}
	else
		exit(ft_atoll(ret));
}

void	end_process(t_data *data, char *ret)
{

	ft_free_tab(data->cmd);
	ft_free_tab(data->paths);
	ft_free_tab(data->env);
	ft_free_tab(data->ghost);
	exit(ft_atoll(ret));
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
	close(data->fd.redir_fd[1]);
	dup2(data->fd.redir_fd[0], 0);
	close(data->fd.redir_fd[0]);
	//close(data->fd.tmp);
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

/*void	add_in_ghost(t_data *data, char *exp)
{
	int	i;
	char	**new;

	i = 0;
	while (data->ghost[i])
	{
		if (ft_strcmp(exp, data->ghost[i]) == 0)
			return ;
		i++;
	}
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
		free_all(data);
	i = 0;
	while (data->ghost[i])
	{
		new[i] = ft_strdup(data->ghost[i]);
		i++;
	}
	new[i] = ft_strdup(exp);
	new[i + 1] = 0;
	free(exp);
	ft_free_tab(data->ghost);
	data->ghost = new;
	printf("ghost\n");
}*/

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
	{
		//char **next;
	 	//add_in_ghost(data, new);
		data->ghost = ft_tabdup(data->ghost, new, 1);
		free(input);
		free(new);
		// ft_free_tab(data->ghost);
		// data->ghost = next;
		return (NULL);
	}
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
	(void)data;
	char	buf[50];
	int	ret;

	ret = 1;
	while (ret)
	{
		ret = read(0, buf, sizeof(buf));
		buf[ret] = '\0';
		write(1, buf, ret);
	}
	/*char	*str;
	char	*str2;
	struct stat stat_info;

	(void)data;
	str = get_next_line(0);
	stat_info.st_mode = 0;
	while (str != NULL)
	{
		str2 = ft_strndup(str, ft_strlen(str) - 1, 0);
		stat(str2, &stat_info);
		if (S_ISDIR(stat_info.st_mode))
			printf (BO_BLUE"%s"RESET, str);
		else if (S_ISREG(stat_info.st_mode) && stat_info.st_mode & S_IXUSR)
			printf (BO_GREEN"%s"RESET, str);
		else
			printf ("%s", str);
		free(str2);
		free (str);
		str = get_next_line(0);
	}
	get_next_line(-99);*/
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
	data->prompt = ft_strjoin(BGO_GREEN BO_BLACK"Minishell~", getenv("USER"), 0);
	data->prompt = ft_strjoin(data->prompt, RESET BO_GREEN"🐸", 1);
	data->prompt = ft_strjoin(data->prompt, cwd, 1);
	data->prompt = ft_strjoin(data->prompt,RESET"\1$ \2", 1);
}

void	edit_paths(t_data *data)
{
    int     i;
    
    i = -1;
    data->paths = ft_split(getenv("PATH"), ':');
	if (!data->paths)
		return ;
    while (data->paths[++i])
		data->paths[i] = ft_strjoin(data->paths[i], "/", 1);
}

void	free_all(t_data *data)
{
	
	rl_clear_history();
	if (data->step == 0)
	{
		free(data->prompt);
		free(data->input);
		ft_free_tab(data->paths);
		ft_free_tab(data->env);
		ft_free_tab(data->ghost);
	}
	if (data->step == 1)
	{
		ft_free_tab(data->paths);
		ft_free_tab(data->env);
		ft_free_tab(data->cmd);
		ft_free_tab(data->ghost);
		close(data->fd.p_fd[0]);
		close(data->fd.p_fd[1]);
		close(data->fd.redir_fd[0]);
		close(data->fd.redir_fd[1]);
	}
	//close(data->fd.tmp);
	close (data->fd.base_fd[0]);
	close (data->fd.base_fd[1]);
	ft_printf("Exiting Minishell\n");
	signals(data, 3);
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
