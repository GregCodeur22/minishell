/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:24:03 by garside           #+#    #+#             */
/*   Updated: 2025/04/18 14:45:52 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_replace_in_env(t_data *data, char *name, char *value)
{
	t_env	*i;

	i = data->env;
	while (i)
	{
		if (ft_strcmp(i->name, name) == 0)
		{
			printf("found\n");
			free(i->content);
			i->content = ft_strdup(value);
			return ;
		}
		i = i->next;
	}
}

int	ft_cd(t_data *data)
{
	char	*home;
	char	cwd[1024];

	if (!data->token->next || data->token->next->type != WORD)
	{
		home = ft_get_env("HOME", data);
		if (!home)
			return (ft_putstr_fd("cd: HOME not set\n", 2), 1);
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			return (ft_putstr_fd("pwd failed\n", 2), 1);
		ft_replace_in_env(data, "OLDPWD", cwd);
		if (chdir(home) == -1)
			return (ft_putstr_fd("chdir failed\n", 2), free(home), 1);
		free(home);
	}
	else if (data->token->next->next && data->token->next->next->type == WORD)
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	else
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			return (ft_putstr_fd("pwd failed\n", 2), 1);
		ft_replace_in_env(data, "OLDPWD", cwd);
		if (chdir(data->token->next->value) == -1)
		{
			ft_putstr_fd("cd: ", 2);
			ft_putstr_fd(data->token->next->value, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (ft_putstr_fd("pwd failed\n", 2), 1);
	ft_replace_in_env(data, "PWD", cwd);
	return (0);
}
