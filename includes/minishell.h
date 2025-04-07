#ifndef LIST_H
# define LIST_H
#include <readline/readline.h>
#include <readline/history.h>
#include "../octolib/includes/libft.h"

#include <signal.h>

#define MAX_TOKEN 100

typedef enum
{
	COMMAND,
	ARGUMENT,
	PIPE,
	REDIRECTION_IN,
	REDIRECTINO_OUT
}	TokenType;

typedef struct s_token
{
	char		*value;
	TokenType 	type;
}	t_token;

typedef struct s_env
{
	char 			*name;
	char 			*content;
	struct s_env 	*next;
	struct s_env 	*prev;
} t_env;

typedef struct s_data
{
	char 		*input;
	t_env 		*env;
	char 		**envp;
	t_token		*token;
	int 		token_count;
} t_data;

void ft_lstadd_back_env(t_env **lst, t_env *new);
void ft_lstadd_back_env(t_env **lst, t_env *new);
int	parse(t_data *data);
int check_quotes(t_data *data);
char	**translate_in_tab(t_data *data);
int calcul_dynamique_len(t_env *tmp);
void free_split(char **tmp);

t_token *ft_lexer(char *input, int *token_count);


//--------gestion des signaux---------
void init_signal(void);

#endif
