/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:12:41 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/23 15:14:36 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		in_ex(t_data *data, char c)
{
	int	i;

	i = 0;
	while (data->ex[i])
	{
		if (c == data->ex[i])
			return (1);
		i++;	
	}
	return (0);
}

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
	//free(str);
    data->env = data->new_env;
}

int	get_nl(char *cmd)
{
	int	i;

	i = 0;
	if (cmd[i] != 'n')
		return (0);
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t')
	{
		if (cmd[i] != 'n')
			return (0);
		i++;
	}
	return (i + 1);
}

void	print_echo(char *cmd)
{
	int	space;
	int	i;

	i = 0;
	while (cmd[i])
	{
		space = 0;
		while (cmd[i] == ' ' || cmd[i] == '\t')
		{
			space = 1;
			i++;
		}
		if (space == 1)
			write(1, " ", 1);
		write(1, &cmd[i], 1);
		i++;
	}
}

static void echo(char *cmd)
{
	int	i;
	int	nl;

	i = 4;
	nl = 0;
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
		i++;
	if (cmd[i] == '-')
			nl = get_nl(cmd + i + 1);
	i += nl;
	while (cmd[i] == ' ' || cmd[i] == '\t')
		i++;
	print_echo(cmd + i);;
	if (!nl)
		write(1, "\n", 1);
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
}

void	print_declare(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		ft_printf("export ");
		ft_printf("%s\n", data->env[i]);
		i++;
	}
	i = 0;
	while (data->ghost[i])
	{
		ft_printf("export ");
		ft_printf("%s\n", data->ghost[i]);
		i++;
	}
}

void	edit_pipe(t_data *data)
{
	int	i;

	i = 0;
	while (data->input[i])
	{
		if (data->input[i] == '|')
			data->pipe++;
		i++;
	}
}

void	end_nonchild(t_data *data)
{
	close (data->fd.base_fd[0]);
	close (data->fd.base_fd[1]);
	if (data->ope)
		free (data->ope);
	//close(data->fd.tmp);
	ft_free_tab(data->cmd);
}

void	cd_manage(t_data *data, char *cmd)
{
	char *path;

	if (cmd[2] == '\0')
	{
		if (get_env(data, "HOME") == NULL)
			ft_dprintf(2, "Minishell: cd: « HOME » not set\n");
		chdir(get_env(data, "HOME"));
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
}

void	recoded(t_data *data, char *cmd)
{
	if (ft_strcmp("pwd", cmd) == 0)
	{
		ft_printf("%s\n", data->cwd);
		end_process(data, "0");
	}
	else if (ft_strcmp("env", cmd) == 0)
	{
		print_env(data);
		end_process(data, "0");
	}
	else if (!ft_strcmp("exit", cmd) || !ft_strncmp("export ", cmd, 7) || !ft_strcmp("cd", cmd) || !ft_strncmp("cd ", cmd, 2) || !ft_strcmp("unset", cmd) || !ft_strncmp("unset ", cmd, 6))
		end_process(data, "0");
	else if (!ft_strncmp("exit ", cmd, 5))
		end_process(data, manage_exit(data, cmd));
	else if (ft_strncmp("echo ", cmd, 5) == 0)
	{
		echo(cmd);
		end_process(data, "0");
	}
	else if (!ft_strcmp("export", cmd))
	{
		end_process(data, "0");
		print_declare(data);
	}

}

int	check_numeric(char *part)
{
	int	i;

	i = 0;
	if (part[0] == '+' || part[0] == '-')
		i++;
	while (part[i])
	{
		if (part[i] < '0' || part[i] > '9')
		{
			ft_dprintf(2, "Minishell: exit: %s: numeric value required\n", part);
			return (1);
		}
		i++;
	}
	return (0);
}

char	*manage_exit(t_data *data, char *cmd)
{
	int	i;
	char	*ret;
	char	**exits;

	ret = NULL;
	i = 0;
	exits = ft_split(cmd, ' ');
	if (check_numeric(exits[1]))
	{
		ft_free_tab(exits);
		return (ft_strdup("2"));
	}
	while (exits[i])
		i++;
	if (i > 2)
	{
		ft_dprintf(2, "exit: too many arguments\n");
		ft_free_tab(exits);
		data->last_ret = 1;
		return (ft_strdup(""));
	}
	if (i == 2)
		ret = ft_strdup(exits[1]);
	ft_free_tab(exits);
	return(ret);
}

int	manage_nonchild(t_data *data)
{
	char	*exit_code;

	if (ft_strncmp("cd", data->cmd[0], 2) == 0 )
	{
		cd_manage(data, data->cmd[0]);
		end_nonchild(data);
		return (1);
	}
	else if (ft_strncmp("unset ", data->cmd[0], 6) == 0)
	{
		data->cmd[0] = parse_unset(data->cmd[0]);
		remove_from_env(data, data->cmd[0]);
		end_nonchild(data);
		return (1);
	}
	else if (ft_strncmp(data->cmd[0], "exit ", 5) == 0)
	{
		exit_code = manage_exit(data, data->cmd[0]);
		end_nonchild(data);
		if (ft_strcmp(exit_code, "") != 0)
		{
			printf("exit code = %s\n", exit_code);
			rl_clear_history();
			ft_free_tab(data->env);
			ft_free_tab(data->ghost);
			end(data, exit_code);
		}
		free(exit_code);
		return (1);
	}

	else if (ft_strcmp(data->cmd[0], "exit") == 0)
	{
		rl_clear_history();
		ft_free_tab(data->env);
		ft_free_tab(data->ghost);
		ft_printf("exit basique\n");
		end_nonchild(data);
		signals(data, 3);
	}
	
	else if (ft_strncmp("export ", data->cmd[0], 7) == 0)
    {
		printf("executing export\n");
		data->cmd[0] = parse_export(data, data->cmd[0]);
		if (data->cmd[0] == NULL)
		{
			end_nonchild(data);
			return (1);
		}
		add_in_env(data, data->cmd[0]);
		end_nonchild(data);
		return (1);
	}
	return (0);
}

char	*get_env(t_data *data, char *macro)
{
	int	i;
	int	j;
	char	*var;
	i = 0;
	while (data->env[i])
	{
		j = 0;
		while (data->env[i][j] != '=')
			j++;
		var = ft_strndup(data->env[i], j, 0);
		if (ft_strcmp(var, macro) == 0)
		{
			free(var);
			return (data->env[i] + j + 1);
		}
		free(var);
		i++;
	}
	return (NULL);
}

char	*get_macro(t_data *data, char *cmd, char *begin)
{
	char	*macro;
	int	i;
	int	j;
	
	i = 0;
	j = -1;
	while (in_ex(data, cmd[i]))
		i++;
	macro = malloc(sizeof(char) * i + 1);
	if (macro == NULL)
		exit(0); // faire une fonction pour exit proprement
	while (++j < i)
		macro[j] = cmd[j];
	macro[j] = '\0';
	//printf("cmd = |%s|\nbegin = |%s|\nmacro = |%s|\n", cmd, begin, macro);
	if (get_env(data, macro) != NULL && (cmd[i] == '\'' || cmd[i] == '"' || cmd[i] == ' ' || cmd[i] == '\\' || !cmd[i]))
		begin = ft_strjoin(begin, get_env(data, macro), 0);
	else
	{
		i = 0;
		while (macro[i])
			i++;
		begin = ft_strjoin(begin, cmd + i, 0);
		//printf("cmd = |%s|\nbegin = |%s|\nmacro = |%s|\n", cmd, begin, macro);
		free(macro);
		return (begin);
	}
	i = 0;
	while (in_ex(data, cmd[i]))
		i++;
	begin = ft_strjoin(begin, cmd + i, 1);
	free(macro);
	return (begin);
}
char	*last_return(t_data *data, char *begin)
{
	char	*itoa_ret;
	
	itoa_ret = ft_itoa(data->last_ret);
	if (itoa_ret == NULL)
		exit(0); // faire une fonction pour exit proprement
	return (ft_strjoin(begin, itoa_ret, 2));
}
char	*ez_money(t_data *data)
{
	int		i;
	int		j;
	char	*begin;
	char	*replaced;

	i = 0;
	j = 0;
	data->lit = 0;
	data->d_lit = 0;
	printf("data->input = %s\n", data->input);
	while (data->input[i])
	{
		if (data->input[i] == '"' && data->lit == 0)
			data->d_lit++;
		data->d_lit %= 2;
		if (data->input[i] == '\'' && data->d_lit == 0)
			data->lit++;
		data->lit %= 2;
		if (data->input[i] == '$' && data->lit == 0)
			break ;
		i++;
	}
	if (!data->input[i])
		return (data->input);
	begin = malloc(sizeof(char) * i + 1);
	if (begin == NULL)
		exit(0); // faire une fonction pour exit proprement
	while (j < i)
	{
		begin[j] = data->input[j];
		j++;
	}
	begin[j] = '\0';
	i++;
	if (data->input[i] == '?' && (data->input[i + 1] == ' ' || data->input[i + 1] == '\t' || data->input[i + 1] == '\0'))
		replaced = last_return(data, begin);
	else
		replaced = get_macro(data, data->input + i, begin);
	free(begin);
	free(data->input);
	//printf("repalced = |%s|\n", replaced);
	return (replaced);
}
