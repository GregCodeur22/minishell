/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:41:37 by garside           #+#    #+#             */
/*   Updated: 2025/06/11 18:19:03 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_split(char **tmp)
{
	int	i;

	i = 0;
	if (tmp)
	{
		while (tmp[i])
		{
			free(tmp[i]);
			i++;
		}
		free(tmp);
	}
}

char	*try_paths(char **paths, char *cmd)
{
	int		i;
	char	*res;
	char	*resfinal;

	i = 0;
	while (paths[i])
	{
		res = ft_strjoin(paths[i], "/");
		if (!res)
			return (NULL);
		resfinal = ft_strjoin(res, cmd);
		free(res);
		if (!resfinal)
		{
			free_split(paths);
			return (NULL);
		}
		if (access(resfinal, X_OK) != -1)
			return (free_split(paths), resfinal);
		free(resfinal);
		i++;
	}
	return (free_split(paths), NULL);
}

char	*find_cmd_path(char *cmd, t_data *data)
{
	char	*path_env;
	char	**paths;
	char	*cmd_path;

	if (cmd[0] == '/')
	{
		if (access(cmd, X_OK) != -1)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path_env = ft_get_env("PATH", data);
	if (!path_env)
		return (ft_strdup(cmd));
	paths = ft_split(path_env, ':');
	free(path_env);
	if (!paths)
		return (NULL);
	cmd_path = try_paths(paths, cmd);
	if (!cmd_path)
		return (ft_strdup(cmd));
	return (cmd_path);
}
