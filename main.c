/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 12:52:35 by injah             #+#    #+#             */
/*   Updated: 2023/06/19 13:06:32 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

void	init(t_data *data, char **env)
{
	data->env = ft_tabdup(env, NULL, 0);
	data->last_ret = 0;
	data->ghost = malloc(sizeof(char *));
	data->ghost[0] = 0;
	data->ex = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
	data->wrong_char = "&;()<>";
	edit_paths(data);
}

void	init_loop(t_data *data)
{
		data->dollar = 0;
		data->last_pid = 0;
		data->step = 0;
		data->fd.heredoc = 0;
		data->fd.append = 0;
		data->pipe = 0;
		data->fd.base_fd[0] = dup(0);
		data->fd.base_fd[1] = dup(1);
		getcwd(data->cwd, sizeof(data->cwd));
		edit_prompt(data, data->cwd);
		//data->fd.tmp = open("/tmp/minishell", O_CREAT | O_TRUNC | O_RDONLY, 0644);
		data->lit = 0;
		data->d_lit = 0;
		data->slash = 0;
}

void	header(void)
{
	struct winsize win;
	int	i;
	int	marge;
	
	marge = ft_strlen(HEADER1) / 2;
	//i = -1;
	ioctl(1, TIOCGWINSZ, &win);
	//while(++i < win.ws_row - 5)
	//	write(1, "\n", 1);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(C_GREEN HEADER1);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(HEADER2);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(HEADER3);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(HEADER4);
	i = -1;
	while (++i < win.ws_col / 2 - marge)
		write(1, " ", 1);
	ft_printf(HEADER5 RESET);
	write(1, "\n\n", 2);
	
}

int	who_is_first(char *str, char *wrong)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	while (str[i])
	{
		j = 0;
		while (wrong[j])
		{
			if (str[i] == wrong[j])
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}

int	chained_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == c)
		i++;
	return (i);
}


int	bracketin_err(char *str, int nb, int dual)
{
	int	i;

	i = 0;
	if (dual)
	{
		ft_dprintf(2, "synthax error near unexpected token « %c »\n", str[dual]);
		return (1);
	}
	if (!str[1] || !str[2] || (!str[3] && str[2] == '<'))
	{
		ft_dprintf(2, "synthax error near unexpected token « newline »\n");
		return (1);
	}
	if (nb > 3)
	{
		ft_dprintf(2, "synthax error near unexpected token « ");
		while (nb > 3 && i < 3)
		{
			ft_dprintf(2, "<");
			nb--;
			i++;
		}
		ft_dprintf(2, " »\n");
		return (1);
	}
	return (0);
}

int	bracketout_err(char *str, int nb, int dual)
{
	int	i;

	i = 0;
	if (dual)
	{
		ft_dprintf(2, "synthax error near unexpected token « %c »\n", str[dual]);
		return (1);
	}
	if (!str[1] || (!str[2] && str[1] == '>'))
	{
		ft_dprintf(2, "synthax error near unexpected token « newline »\n");
		return (1);
	}
	if (nb > 2)
	{
		ft_dprintf(2, "synthax error near unexpected token « ");
		while (nb > 2 && i < 2)
		{
			ft_dprintf(2, ">");
			nb--;
			i++;
		}
		ft_dprintf(2, " »\n");
		return (1);
	}
	return (0);
}

int	dual(char *input, char *wrong, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		j = 0;
		while (wrong[j])
		{
			if (wrong[j] == input[i] && input[i] != c)
				return (i);
			j++;
		}
		i++;
	}
	return (0);
	
}

int	stx_error(t_data *data, char *input)
{
	int	i;
	int	j;
	int	k;

	k = 0;
	i = who_is_first(input, data->wrong_char);
	if (i == -1)
		return (0);
	j = chained_char(input + i, input[i]);
	if (input[i] != '<' && input[i] != '>')
	{
		ft_dprintf(2, "synthax error near unexpected token « ");
		while (j && k < 2)
		{
			ft_dprintf(2, "%c", input[i]);
			j--;
			k++;
		}
		ft_dprintf(2, " »\n");
		return (1);
	}
	//k = dual(input + i, data->wrong_char, input[i]);
	if (input[i] == '<')
		j = bracketin_err(input + i, j, k);
	else if (input[i] == '>')
		j = bracketout_err(input + i, j, k);
	return (j);
}
/*si on ouvre un fd ou rajoute un malloc, il faut le fermer dans "end_process" "free_all" et "end_nonchild*/
int	main(int ac, char **av, char **env) 
{
	t_data	data;
	int	ret;
	(void)ac;
	(void)av;
	int status;


	
	header();
	init(&data, env);
	while (1) 
	{
		ret = 0;
		init_loop(&data);
		//printf("tty = %d\n", ttyslot());
		signals(&data, 1);
		data.input = readline(data.prompt);
		add_history(data.input);
		if (data.input == NULL)
			free_all(&data);
		data.input = ft_strtrim(data.input, " \t;!", 1);
		edit_dollar(&data);
		while (data.dollar--)
			data.input = ez_money(&data);
		if (!ft_strcmp(data.input, "") || stx_error(&data, data.input))
		{
			//printf("hello\n");
			close (data.fd.base_fd[0]);
			close (data.fd.base_fd[1]);
			//close(data.fd.tmp);
			free(data.prompt);
			free(data.input);
			continue;
		}
		
		
		edit_pipe(&data);							//ne pas bouger l'ordre des fonctions, sinon bug =)
		data.cmd = ft_split(data.input, '|');
		free(data.input);
		free(data.prompt);
		
		if (data.pipe == 0)
			ret = manage_nonchild(&data);
		if (ret == 1)
			continue;
		//printf("hello\n");
		execution(&data);
		if (data.last_pid)
			waitpid(data.last_pid, &status, 0);								//bien laisser les free avant de creer les child sinon on duplique les heaps et bug
		while (wait(NULL) > 0)
			;
		data.last_ret = WEXITSTATUS(status);
		if (!isatty(0))
			print(&data);
		//dprintf(2, "%d\n", getpid());
		ft_free_tab(data.cmd);
		//ft_dprintf(2, "retablissement des fd\n");
		dup2(data.fd.base_fd[0], 0);
		close(data.fd.base_fd[0]);
		dup2(data.fd.base_fd[1], 1);
		close(data.fd.base_fd[1]);
		//close(data.fd.tmp);
	}
	return (0);
}
