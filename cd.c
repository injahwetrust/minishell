/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 12:25:47 by bvaujour          #+#    #+#             */
/*   Updated: 2023/07/14 14:30:34 by bvaujour         ###   ########.fr       */
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

static int	cd_path(t_data *data, char *path)
{
	int	ret;
	char	*pwd;
	
	pwd = ft_strjoin("OLDPWD=", data->cwd, 0);
	if (!pwd)
		end(data);
	if (ft_strcmp(path, "-") == 0)
	{
		ret = chdir(get_env(data, "OLDPWD"));
		if (ret != 0)
			return (dprintf(2, "Minishell: cd: %s: %s\n", path, strerror(errno)));
		else
			add_in_env(data, pwd);
	}
	else
	{
		ret = chdir(path);
		if (ret)
			return (dprintf(2, "Minishell: cd: %s: %s\n", path, strerror(errno)));
		else
			add_in_env(data, pwd);
	}
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
