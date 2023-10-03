/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 12:25:47 by bvaujour          #+#    #+#             */
/*   Updated: 2023/09/20 13:31:50 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/minishell.h"

static int	cd_home(t_data *data)
{
	char	*pwd;

	if (get_env(data, "HOME") == NULL)
	{
		ft_dprintf(2, "Minishell: cd: « HOME » not set\n");
		return (1);
	}
	chdir(get_env(data, "HOME"));
	pwd = ft_strjoin("OLDPWD=", data->cwd, 0);
	if (!pwd)
		end(data);
	return (add_in_env(data, pwd), free(pwd), 0);
}

static int	cd_error(void)
{
	char	*str;

	str = getcwd(NULL, 0);
	if (str == NULL)
	{
		ft_dprintf(2, CHDIR CD_ERR_CHDIR, strerror(errno));
		free (str);
		return (1);
	}
	free (str);
	return (0);
}

static int	cd_path(t_data *data, char *path)
{
	int		ret;
	char	*pwd;

	if (ft_strcmp(path, "-") == 0)
	{
		if (get_env(data, "OLDPWD"))
		{
			printf("%s\n", get_env(data, "OLDPWD"));
			ret = chdir(get_env(data, "OLDPWD"));
		}
		else
			return (ft_dprintf(2, "Minishell: cd: « OLDPWD » undefined\n"));
	}
	else
	{
		ret = chdir(path);
		if (cd_error())
			return (1);
		if (ret)
			return (ft_dprintf(2, CD_ERR_1, path, strerror(errno)));
	}
	pwd = ft_strjoin("OLDPWD=", data->cwd, 0);
	if (!pwd)
		end(data);
	return (add_in_env(data, pwd), free(pwd), 0);
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
	if (i == 1 || (i == 2 && (!ft_strcmp("--", s_cmd[1])
				|| !ft_strcmp("~", s_cmd[1]))))
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
