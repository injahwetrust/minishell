/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:12:41 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/10 16:29:58 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_in_env(t_data *data, char *str)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (!strncmp(str, data->env[i], ft_strlen(str)) && data->env[i][ft_strlen(str)] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	remove_from_env(t_data *data, char *str)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	if (!is_in_env(data, str))
		return ;
	while (data->env[i])
		i++;
	data->new_env = malloc(sizeof(char *) * i);
	if (!data->new_env)
		exit(0); // faire une fonction pour exit proprement
	i = 0;
	while (data->env[i])
	{
		if (!strncmp(str, data->env[i], ft_strlen(str)))
		{
			ft_dprintf(2, "removed\n");
			free (data->env[i]);
			i++;
		}
		data->new_env[j] = data->env[i];
		if (!data->env[i])
			break;
		j++;
		i++;
	}
	data->new_env[j] = 0;
	free(data->env);
	data->env = data->new_env;
}

int	replace_in_env(t_data *data, char *str)
{
	int	i;
	int j;
	
	i = 0;
	j = 0;
	while (str[i] != '=')
		i++;
	while (data->env[j])
	{
		if (ft_strncmp(data->env[j], str, i) == 0)
		{
			free(data->env[j]);
			data->env[j] = ft_strdup(str);
			return (1);
		}
		j++;
	}
	return (0);
}

void    add_in_env(t_data *data, char *str)
{
    int i;
	
	if (replace_in_env(data, str))
		return ;
    i = 0;
    while (data->env[i])
        i++;
    data->new_env = malloc(sizeof(char *) * (i + 2));
    i = -1;
    while (data->env[++i])
	{
        data->new_env[i] = data->env[i];
	}
	data->new_env[i] = ft_strdup(str);
    data->new_env[i + 1] = 0;
	free(data->env);
    data->env = data->new_env;
}

static void echo(t_data *data, char *cmd)
{
	int	i;
	int	nl;

	i = 3;
	nl = 1;
	while (cmd[++i])
	{
		if (cmd[i] == '-' && cmd[i + 1] == 'n' && (cmd[i + 2] == ' ' || cmd[i + 2] == '\t'))
			nl = 0;
		if (cmd[i] != ' ' && cmd[i] != '\t')
			break;
	}
	if (nl == 0)
		while (cmd[i] != 'n')
			i++;
	else
		i = 3;
	i++;
	while (cmd[i] == ' ' || cmd[i] == '\t')
		i++;
	while (cmd[i])
	{
		ft_printf("%c", cmd[i]);
		i++;
	}
	if (nl == 1)
		ft_printf("\n");
	end_process(data);
}

void	print_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		ft_printf("%s\n", data->env[i]);
		i++;
	}
	end_process(data);
}

void	edit_pipe(t_data *data, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			data->pipe++;
		i++;
	}
}

void	cd_manage(t_data *data, char *cmd)
{
	char *path;

	if (cmd[2] == '\0')
	{
		chdir("/");
		close (data->base_fd[0]);
		close (data->base_fd[1]);
		return ;
	}
	path = ft_strdup(cmd + 2);
	if (!path)
		exit(ft_dprintf(2, "Malloc error\n")); // faire une fonction pour exit proprement
	path = ft_strtrim(path, " \t", 1);
	if (!path)
		exit(ft_dprintf(2, "Malloc error\n")); // faire une fonction pour exit proprement
	ft_dprintf(2, "path = %s\n", path);
	chdir(path);
	close (data->base_fd[0]);
	close (data->base_fd[1]);
}

void	recoded(t_data *data, char *cmd)
{

	if (ft_strcmp("pwd", cmd) == 0)
	{
		ft_printf("%s\n", data->cwd);
		end_process(data);
	}
	else if (ft_strcmp("env", cmd) == 0)
		print_env(data);
	else if (ft_strcmp("exit", cmd) == 0 || ft_strncmp("export", cmd, 6) == 0 || ft_strncmp("cd", cmd, 2) == 0)
		end_process(data);
	else if (ft_strncmp("echo", cmd, 4) == 0)
		echo(data, cmd);

}

int	manage_nonchild(t_data *data, char *input)
{
	int	ret;

	ret = 0;
	if (ft_strncmp("cd", input, 2) == 0 && data->pipe == 0)
	{
		cd_manage(data, input);
		free(data->prompt);
		free(input);
		close (data->base_fd[0]);
		close (data->base_fd[1]);
		ret = 1;
	}
	else if (ft_strncmp("unset", input, 5) == 0 && data->pipe == 0)
	{
		input = parse_unset(input);
		remove_from_env(data, input);
		free(data->prompt);
		free(input);
		close (data->base_fd[0]);
		close (data->base_fd[1]);
		ret = 1;
	}
	else if (!ft_strcmp(input, "exit"))
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
	else if (ft_strncmp("export", input, 6) == 0 && data->pipe == 0)
    {
		input = parse_export(input);
		if (input == NULL)
		{
			ret = 1;
			free(input);
			free(data->prompt);
			close (data->base_fd[0]);
			close (data->base_fd[1]);
			return (ret);
		}
        add_in_env(data, input);
		ret = 1;
		free(data->prompt);
		close (data->base_fd[0]);
		close (data->base_fd[1]);
		free(input);
	}
	return (ret);
}

char	*replace_dollar(char *cmd, char *macro, int size, t_data *data)
{
	char	*to_add;
	char	*final;
	int		i;
	int		j;
	int		k;
	
	i = -1;
	j = 0;
	to_add = ft_strdup(getenv(macro));
	final = malloc(sizeof(char) * size + 1);
	if (!final || !to_add)
		end_process(data);
	while (cmd[++i] != '$')
		final[i] = cmd[i];
	k = i + ft_strlen(macro) + 1;
	while (to_add[j])
		final[i++] = to_add[j++];
	while (cmd[k])
		final[i++] = cmd[k++];
	final[i] = '\0';
	free(cmd);
	return (free(to_add), final);
}

char	*ez_money(t_data *data, char *cmd)
{
	int	i;
	int	j;
	int	total_size;
	char	*macro;
	
	i = -1;
	j = 0;
	while (cmd[++i])
		if (cmd[i] == '$')
			break;
	if (!cmd[i])
		return (cmd);
	i++;
	while (cmd[i + j] != ' ' &&  cmd[i + j] != '\t' && cmd[i + j])
		j++;
	macro = malloc(sizeof(char) * (j + 1));
	if (macro == NULL)
		end_process(data);
	macro[j] = '\0';
	ft_strlcpy(macro, cmd + i, j);
	total_size = ft_strlen(cmd) + ft_strlen(getenv(macro)) - (j + 1);
	cmd = replace_dollar(cmd, macro, total_size, data);
	return (free(macro), cmd);
}