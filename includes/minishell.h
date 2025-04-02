#ifndef LIST_H
# define LIST_H
#include <readline/readline.h>
#include <readline/history.h>
# include "../octolib/includes/libft.h"

typedef struct s_env
{
	char *name;
	char *content;
	struct s_env *next;
	struct s_env *prev;
} t_env;

typedef struct s_data
{
	char 		*input;
	t_env 	*env;
	char 		**envp;
} t_data;

void ft_lstadd_back_env(t_env **lst, t_env *new);
//void find_quote(t_data *data);

#endif