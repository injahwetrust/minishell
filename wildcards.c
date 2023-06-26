/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 23:30:16 by bvaujour          #+#    #+#             */
/*   Updated: 2023/06/26 15:10:58 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*all_dir(void)
{
	DIR	*dir;
	struct dirent *entry;
	char	*str;

	str = ft_strdup("");
	
	dir = opendir(".");
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.')
		{
			str = ft_strjoin(str, ft_strdup(entry->d_name), 3);
			str = ft_strjoin(str, " ", 1);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (ft_strcmp(str, "") == 0)
		return (ft_strdup("*"));
	return (str);
}

int	ast_size(t_data *data, char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '"' && !data->lit)
			data->d_lit++;
		if (cmd[i] == '\'' && !data->d_lit)
			data->lit++;
		data->lit %= 2;
		data->d_lit %= 2;
		if (cmd[i] == ' ' && !data->lit && !data->lit)
			break ;
	}
	return(i);
}

int	reverse_ast(char *ast, char *name)
{
	int	i;
	int	j;
	
	i = 0;
	while (ast[i] == '*')
		i++;
	if (!ast[i])
		return (1);
	i = ft_strlen(ast);
	j = ft_strlen(name);
	while (ast[i] != '*')
	{
		//printf("reverse check\n");
		while (ast[i] == '"' || ast[i] == '\'')
			i--;
		if (ast[i] != name[j])
			return (0);
		if (ast[i] == '*')
			break ;
		i--;
		j--;
	}
	return (1);
}
int	match_ast(char *ast, char *name)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	//printf("ast = %s\n", ast);
	while (ast[i])
	{
		while (ast[i] == '"' || ast[i] == '\'')
			i++;
		if (ast[i] == '*')
		{
			if (reverse_ast(ast + i, name))
				return (1);
			else
				return (0);
		}
		if (ast[i++] != name[j++])
			return (0);
	}
	return (1);
}

char	*replace_ast(char *ast)
{
	DIR	*dir;
	struct dirent *entry;
	char	*str;
	
	str = ft_strdup("");
	dir = opendir(".");
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (match_ast(ast, entry->d_name))
		{
			str = ft_strjoin(str, ft_strdup(entry->d_name), 3);
			str = ft_strjoin(str, " ", 1);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	if (ft_strcmp(str, "") == 0)
	{
		free (str);
		return (ast);
	}
	free (ast);
	return (str);
}

char	*seg_dir(t_data *data, char *cmd)
{
	int	len;
	char	*ast;

	len = ast_size(data, cmd);
	//printf("len = %d\n", len);
	ast = malloc(sizeof(char) * len + 1);
	if (!ast)
		exit(0); //exit proprement
	ft_strlcpy(ast, cmd, len);
	//printf("ast = %s\n", ast);
	ast = replace_ast(ast);
	return(ast);
}

char	*wildcards(t_data *data, char *cmd)
{
	int	i;
	char	*new;
	(void)data;
	
	new = ft_strdup("");
	i = 0;
	data->lit = 0;
	data->d_lit = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"' && !data->lit)
			data->d_lit++;
		if (cmd[i] == '\'' && !data->d_lit)
			data->lit++;
		data->lit %= 2;
		data->d_lit %= 2;
		if (cmd[i] == '*' && !data->lit && !data->d_lit && (cmd[i + 1] == ' ' || cmd[i + 1] == '\0') && (i == 0 || cmd[i - 1] == ' '))
			new = ft_strjoin(new, all_dir(), 3);
		else if (cmd[i] == '*' && !data->lit && !data->d_lit)
		{
		 	while (i > 0 && cmd[i] != ' ')
		 		i--;
			i++;
			new = ft_strjoin(ft_strndup(new, i, 1), seg_dir(data, cmd + i), 3);
			while (cmd[i])
			{
				if (cmd[i] == '"' && !data->lit)
					data->d_lit++;
				if (cmd[i] == '\'' && !data->d_lit)
					data->lit++;
				data->lit %= 2;
				data->d_lit %= 2;
				if ((cmd[i] == ' ' || !cmd[i]) &&  !data->lit && !data->d_lit)
					break ;
				i++;
			}
		}
		if (!cmd[i])
			break ;
		else
			new = ft_strjoin(new, ft_strndup(cmd + i, 1, 0), 3);
		i++;
	}
	
	free(cmd);
	//printf("hello new = |%s|\n", new);
    return (new);
}

/*char	*wildcards(t_data *data, char *cmd)
{
	DIR	*dir;
	struct dirent *entry;
	int	i;
	char	**spl;
	char	*new;
	(void)data;
	
	new = ft_strdup("");
	i = 0;
	spl = ft_split(cmd, ' ');
	data->lit = 0;
	data->d_lit = 0;
	while (cmd[i])
	{
		//printf("spl[%d] = |%s|\n", i, spl[i]);
		
		if (spl[i][0] == '*' && !spl[i][1])
			new = ft_strjoin(new, all_dir(new), 3);
		// if (spl[i][0] == '*' && !spl[i][1])
		// {
		// 	dir = opendir(".");
		// 	entry = readdir(dir);
		// 	while (entry != NULL)
		// 	{
		// 		if (entry->d_name[0] != '.')
		// 		{
		// 			//printf("%s\n", entry->d_name);
		// 			new = ft_strjoin(new, ft_strdup(entry->d_name), 3);
		// 			new = ft_strjoin(new, " ", 1);
		// 		}
		// 		entry = readdir(dir);
    	// 	}
		// 	closedir(dir);
		// }
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
}*/