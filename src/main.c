#include <stdio.h>
#include <stdlib.h>
#include "../includes/minishell.h"
#include "../octolib/includes/libft.h" 

t_env	*env_new(char *name, char *value)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	new_env->name = ft_strdup(name);
	new_env->content = ft_strdup(value);
	new_env->next = NULL;
	new_env->prev = NULL;
	return (new_env);
}

void	free_env_list(t_env *new_list)
{
	t_env	*temp;

	while (new_list)
	{
		temp = new_list;
		new_list = new_list->next;
		free(temp->name);
		free(temp->content);
		free(temp);
	}
}

t_env *init_env_list(char **env)
{
	t_env *env_list;
	t_env *new_env;
	char *sep;
	int i;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		sep = ft_strchr(env[i], '=');
		if (sep)
		{
			*sep = '\0';
			new_env = env_new(env[i], sep + 1);
			*sep = '=';
			if (!new_env)
			{
				i++;
				continue;
			}
			ft_lstadd_back_env(&env_list, new_env);
		}
		i++;
	}
	return (env_list);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_data data;
	// char **tab_env;

	data.envp = env;
	data.env = init_env_list(env);
	data.last_status = 0;
	// tab_env = translate_in_tab(&data);
	// free_split(tab_env);
	init_signal();
	while (1)
	{
		data.token = NULL;
		data.input = readline("minishell> ");
		if (!data.input)
		{
			printf("exit\n");
			break;
		}
		if (data.input[0])
		{
			parse(&data);
			exec_line(&data);
			add_history(data.input);
			if (data.token)
				free_token(data.token);
		}
		free(data.input);
	}
	free_env_list(data.env);
	rl_clear_history();
	return (0);
}

