#ifndef LIST_H
# define LIST_H
#include <readline/readline.h>
#include <readline/history.h>
# include "../octolib/includes/libft.h"

#include <signal.h>

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
void ft_lstadd_back_env(t_env **lst, t_env *new);
int	parse(t_data *data);
int check_quotes(t_data *data);
char	**translate_in_tab(t_data *data);
int calcul_dynamique_len(t_env *tmp);
void free_split(char **tmp);


//--------gestion des signaux---------
void init_signal(void);
#endif
