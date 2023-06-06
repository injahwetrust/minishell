/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:52:35 by injah             #+#    #+#             */
/*   Updated: 2023/06/06 19:41:43 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


typedef struct s_data
{
	char	**paths;
	char	**env;
	char	*cur_dir;
	char	*prompt;
	


	int	pipe;
}	t_data;


void	ft_free_tab(char **tab)
{
	int	i;
	
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

char	*get_exec(char *cmd, t_data *data)
{
	int	i;
	char	*path;

	i = -1;
	while (data->paths[++i])
	{
		path = ft_strjoin(data->paths[i], cmd, 1);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
	}
	return (cmd);
}

void	exec(char *cmd, t_data *data)
{
	char	**s_cmd;
	char	*path;
	pid_t	pid;
	
	pid = fork();
	if (pid == 0)
	{
		s_cmd = ft_split(cmd, ' ');
		path = get_exec(s_cmd[0], data);
		if (execve(path, s_cmd, data->env) <= -1)
		{
			//errno = ESRCH;
			//perror(s_cmd[0]);
			printf("command not found: %s\n",s_cmd[0]);
			free(cmd);
			ft_free_tab(s_cmd);
			free(data->paths);
			exit(0);
		}
	}
	else
		waitpid(pid, NULL, 0);
}

void	edit_paths(t_data *data)
{
    int     i;
    
    i = -1;
    data->paths = ft_split(getenv("PATH"), ':');
    while (data->paths[++i])
		data->paths[i] = ft_strjoin(data->paths[i], "/", 1);
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
	data->prompt = ft_strjoin(data->prompt, RESET BO_GREEN" ", 1);
	data->prompt = ft_strjoin(data->prompt, cwd, 1);
	data->prompt = ft_strjoin(data->prompt,RESET"$ ", 1);
}

void	edit_pipe(t_data *data, char *trim)
{
	int	i;
	
	i = -1;
	while (trim[++i])
		if (trim[i] == '|')
			data->pipe++;
}

int main(int ac, char **av, char **env) 
{
	t_data	data;
	char* input;
	char cwd[PATH_MAX];
	char* trim;

	(void)ac;
	(void)av;
	data.env = env;
	edit_paths(&data);
	data.pipe = 0;
	while (1) 
	{
		getcwd(cwd, sizeof(cwd));
		edit_prompt(&data, cwd);
		input = readline(data.prompt);
		if (input == NULL)
		{
			free(data.prompt);
			free(input);
			break;
		}
		trim = ft_strtrim(input, " \t");
		if (trim == NULL || !ft_strncmp(trim, "", 1))
		 	continue;
		edit_pipe(&data, trim);
		if (ft_strncmp("pwd", trim, 3) == 0)
			printf("%s\n", cwd);
		else if (ft_strncmp("cd", trim, 2) == 0)
		{
			trim = ft_strtrim(trim + 2, " \t");
			chdir(trim);
		}
		else
			exec(trim, &data);
		add_history(input);
		free(input);
		free(data.prompt);
		free(trim);
	}
	ft_free_tab(data.paths);
	return (0);
}
