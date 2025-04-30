#include "../includes/minishell.h"
#include "../octolib/includes/libft.h" 

int calcul_dynamique_len(t_env *tmp)
{
	int len_name;
	int	len_content;

	len_name = ft_strlen(tmp->name);
	len_content = ft_strlen(tmp->content);
	return (len_name + len_content + 2);
}

int size_list(t_env *tmp)
{
	int	count;

	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**translate_in_tab(t_data *data)
{
	int	i;
	int len;
	char **envp;
	char *line;
	t_env	*tmp;
	i = 0;
	tmp = data->env;
		
	i = size_list(tmp);
	envp = malloc(sizeof(char *) * (i + 1));
	if (!envp)
	{
		ft_putstr_fd("Error\n:allocation fail\n", 2);
		return (NULL);
	}
	tmp = data->env;
	i = 0;
	while (tmp)
	{
		len = calcul_dynamique_len(tmp);
		line = malloc(len);
		if (!line)
		{
			free_split(envp);
			return (NULL);
		}
		ft_strlcpy(line, tmp->name, len);
		ft_strlcat(line, "=", len);
		ft_strlcat(line, tmp->content, len);
		envp[i] = line;
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

