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
	return (new_env);
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
			free(new_env);
		}
		i++;
	}
	return (env_list);
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_data data;
	t_env *new_node;

	new_node = init_env_list(env);
	while (new_node)
	{
		printf("%s=%s\n", new_node->name, new_node->content);
		free(new_node->name);
		free(new_node->content);
		new_node = new_node->next;
	}
	free(new_node);
	data.input = readline("minishell> ");
	while (data.input)
	{
		printf("%s\n", data.input);
		add_history(data.input);
		free(data.input);
		data.input = readline("minishell> ");
	}
	rl_clear_history();
	return (0);
}
