/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:41:37 by garside           #+#    #+#             */
/*   Updated: 2025/04/17 17:04:37 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_path_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
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
			return (NULL);
		if (access(resfinal, X_OK) != -1)
		{
			free_split(paths);
			return (resfinal);
		}
		free(resfinal);
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **env)
{
	char	*path_env;
	char	**paths;
	char	*cmd_path;

	if (cmd[0] == '/')
	{
		if (access(cmd, X_OK) != -1)
			return (cmd);
		else
			return (NULL);
	}
	path_env = get_path_env(env);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	cmd_path = try_paths(paths, cmd);
	if (!cmd_path)
		return (NULL);
	return (cmd_path);
}

