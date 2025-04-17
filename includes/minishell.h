#ifndef LIST_H
# define LIST_H
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include "../octolib/includes/libft.h"

#include <signal.h>
#include <unistd.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECTION_IN,
	REDIRECTION_OUT,
	HEREDOC,
	APPEND,
}	TokenType;

typedef struct s_token
{
	char			*value;
	TokenType 	type;
	struct s_token *next;
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
	int			last_status;
} t_data;

void	free_env_list(t_env *new_list);
void ft_lstadd_back_env(t_env **lst, t_env *new);
void ft_lstadd_back_env(t_env **lst, t_env *new);
int	parse(t_data *data);
char	**translate_in_tab(t_data *data);
int calcul_dynamique_len(t_env *tmp);
void free_split(char **tmp);
char	*ft_get_env(char *str, t_data *data);
char	*change_env(t_data *data, int *i);
t_token *ft_lexer(t_data *data);
int check_quotes(char *input);
void	free_token(t_token *head);
char	*handle_quotes(t_data *data, int *i);
t_token *new_token(char *value, TokenType type);
char	*extract_word_double(t_data *data, int *i);
t_token  *handle_cmd_or_arg(t_data *data, int *i);


//--------gestion des signaux---------
void init_signal(void);

//-------------builtins---------------
int	ft_pwd(void);
int	ft_cd(t_data *data);
int	ft_env(t_data *data);
int	ft_echo(t_data *data);

// ryew
int	ft_executables(t_data *data);
void	free_data(t_data *data);
char	**ft_get_cmd(t_data *data);
int exec_line(t_data *data);
char	*find_cmd_path(char *cmd, char **env);

#endif
