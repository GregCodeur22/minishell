/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:24:03 by garside           #+#    #+#             */
/*   Updated: 2025/04/30 13:53:22 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	update_pwd_env(t_data *data)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (ft_putstr_fd("pwd failed\n", 2), 1);
	ft_replace_in_env(data, "PWD", cwd);
	return (0);
}

int	update_oldpwd_and_chdir(t_data *data, char *path)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (ft_putstr_fd("pwd failed\n", 2), 1);
	ft_replace_in_env(data, "OLDPWD", cwd);
	if (chdir(path) == -1)
		return (ft_putstr_fd("chdir failed\n", 2), 1);
	return (0);
}

int	cd_to_home(t_data *data)
{
	char	*home;

	home = ft_get_env("HOME", data);
	if (!home)
		return (ft_putstr_fd("cd: HOME not set\n", 2), 1);
	if (update_oldpwd_and_chdir(data, home))
		return (free(home), 1);
	free(home);
	return (0);
}

int	cd_to_path(t_data *data)
{
	char	*path;

	path = data->token->next->value;
	if (update_oldpwd_and_chdir(data, path))
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

int	ft_cd(t_data *data)
{
	if (!data->token->next || data->token->next->type != WORD)
	{
		if (cd_to_home(data))
			return (1);
	}
	else if (data->token->next->next && data->token->next->next->type == WORD)
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	else
	{
		if (cd_to_path(data))
			return (1);
	}
	return (update_pwd_env(data));
}
