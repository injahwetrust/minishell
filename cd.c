/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 12:25:47 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/20 02:55:38 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_home(t_data *data)
{
	if (get_env(data, "HOME") == NULL)
	{
		dprintf(2, "Minishell: cd: « HOME » not set\n");
		return (1);
	}
	chdir(get_env(data, "HOME"));
	return (0);
}

static int cd_error()
{
	char	*str;
	
	str = getcwd(NULL, 0);
	if (str == NULL)
	{
		dprintf(2, "chdir: error retrieving current directory: getcmd: cannot access parent directories: %s\n", strerror(errno));
		free (str);
		return (1);
	}
	free (str);
	return (0);
}

static int	cd_path(t_data *data, char *path)
{
	int	ret;
	char	*pwd;

	if (ft_strcmp(path, "-") == 0)
	{
		ret = chdir(get_env(data, "OLDPWD"));
		if (ret != 0)
			return (dprintf(2, "Minishell: cd: « OLDPWD » undefined\n"));
	}
	else
	{
		ret = chdir(path);
		if (cd_error())
			return (1);
		if (ret)
			return (dprintf(2, "Minishell: cd: %s: %s\n", path, strerror(errno)));
	}
	pwd = ft_strjoin("OLDPWD=", data->cwd, 0);
	if (!pwd)
		end(data);
	add_in_env(data, pwd);
	free(pwd);
	return (0);
}

int	cd(t_data *data, char **s_cmd)
{
	int	i;

	i = 0;
	while (s_cmd[i])
		i++;
	if (i > 2)
	{
		errno = E2BIG;
		perror("Minishell: cd");
		return (1);
	}
	if (i == 1)
	{
		if (cd_home(data))
			return (1);
	}
	else if (i == 2)
	{
		if (cd_path(data, s_cmd[1]))
			return (1);
	}
	return (0);
}
