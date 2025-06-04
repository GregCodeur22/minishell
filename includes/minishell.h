/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:20:24 by garside           #+#    #+#             */
/*   Updated: 2025/06/04 15:19:57 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../octolib/includes/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# define SUCCESS 0
# define FAIL 1
# define CODE_FAIL 1
# define CODE_SUCCESS 0
# define PIPE_READ 0
# define PIPE_WRITE 1
# define ERR_SYNT "minishell: syntax error near unexpected token"

extern volatile sig_atomic_t	g_status;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECTION_IN,
	REDIRECTION_OUT,
	HEREDOC,
	APPEND,
}								t_TokenType;

typedef struct s_token
{
	char						*value;
	t_TokenType					type;
	struct s_token				*next;
}								t_token;

typedef struct s_env
{
	char						*name;
	char						*content;
	struct s_env				*next;
	struct s_env				*prev;
}								t_env;

typedef struct s_redir
{
	char						*file;
	int							type;
	struct s_redir				*next;
}								t_redir;

typedef struct s_cmd
{
	char						**args;
	char						*path;
	t_redir						*infile;
	t_redir						*outfile;
	int							here_doc_mode;
	int							pipe_fd[2];
	int							saved_stdin;
	int							saved_stdout;
	struct s_cmd				*next;
}								t_cmd;

typedef struct s_data
{
	char						*input;
	t_env						*env;
	t_env						*export;
	char						**envp;
	t_token						*token;
	t_cmd						*cmd_list;
	int							last_status;
	int							token_count;
}								t_data;

// signaux
void							reset_signals_child(void);

// parse
t_env							*env_new(char *name, char *value);
void							free_env_list(t_env *new_list);
t_token							*get_next_token(t_data *data, int *i);
t_token							*ft_lexer(t_data *data);
int								parse(t_data *data);

// lexer
char							*handle_quotes_part(t_data *data, int *i,
									char *value);
t_token							*handle_cmd_or_arg(t_data *data, int *i);
t_token							*handle_double_redir(char *input, int *i);
t_token							*handle_redirection(char *input, int *i);
void							skip_whitespace(const char *input, int *i);

// lexer1
void							free_one_token(t_token *token);
void							free_token(t_token *head);
t_token							*new_token(char *value, t_TokenType type);
char							*handle_error_code(t_data *data, char *value,
									int *i);
void							add_token_to_list(t_token **head,
									t_token **last, t_token *new_token);

// lexer2
int								is_skippable_char(char c);
int								is_token_char(char c);
char							*handle_env_value(t_data *data, int *i,
									char *value);
char							*handle_plain_text(t_data *data, int *i,
									char *value);
t_token							*handle_pipe(int *i);

// quote
char							*append_env_variable(t_data *data,
									char *extract, int *i, int *first);
char							*append_remaining_segment(t_data *data,
									char *extract, int first, int i);
char							*extract_word_double(t_data *data, int *i);
char							*extract_word_single(char *input, int *i);
char							*handle_quotes(t_data *data, int *i);

// quote1
int								check_quotes(char *input);
char							*ft_get_env(char *str, t_data *data);
char							*change_env(t_data *data, int *i);
char							*append_error_code(t_data *data, char *extract,
									int *i, int *first);

// exec
char							*get_cmd_path(t_data *data, char **cmd);
int								exec_child_process(t_data *data, t_cmd *cmd,
									int prev_fd);
int								ft_shell(t_data *data, t_cmd *cmd, int prev_fd);
int								which_command(t_data *data, t_cmd *cmd,
									int prev_fd);
int								handle_single_command(t_data *data, t_cmd *cmd,
									int prev_fd);
void							handle_useless_command(t_cmd *cmd,
									int *prev_fd);
int								wait_for_children(t_data *data, pid_t last_pid);
void							maybe_close(t_cmd *cmd, int *prev_fd);
int								exec_line(t_data *data, t_cmd *cmd);

// parse
void							print_cmds(t_cmd *cmd);
t_cmd							*parse_tokens(t_data *data);
t_cmd							*new_cmd_node(void);
void							free_cmd_list(t_data *data);

// parse_utils
void							add_arg(t_cmd *cmd, char *value);
void							add_redir(t_redir **redir_list, char *filename,
									int type, int *skip_next_word);
void							create_parse(t_token *token, t_cmd **curr,
									int *skip_next_word);
void							loop_parse(t_token *token, t_cmd **curr,
									t_cmd **head, int *skip_next_word);
t_cmd							*parse_tokens(t_data *data);

// parse_utils1
void							free_redir_list(t_redir *redir);
void							free_cmd_list(t_data *data);
t_cmd							*new_cmd_node(void);

// exec1
void							free_data(t_data *data);
char							**ft_get_cmd(t_data *data);
void							ft_replace_in_env(t_data *data, char *name,
									char *value);

// export
int								ft_is_valid(char *str);
int								check_name(char *str, t_env *node,
									char *content);
void							free_name_content(char *name, char *content);
char							*get_content(char *str, int i);

// char	*find_cmd_path(char *cmd, t_data *data);
void							ft_lstadd_back_env(t_env **lst, t_env *new);
char							**translate_in_tab(t_data *data);
int								calcul_dynamique_len(t_env *tmp);
void							free_split(char **tmp);
t_env							*init_env_list(char **env);

//--------gestion des signaux---------
void							init_signal(void);

//-------------builtins---------------
int								ft_pwd(void);
int								ft_cd(t_data *data);
int								ft_env(t_data *data);
int								ft_echo(t_cmd *cmd);
int								ft_exit(t_data *data, t_cmd *cmd, int stdin,
									int stdout);
int								ft_isalldigit(char *str);

// ryew
int								ft_executables(t_data *data, t_cmd *cmd,
									int input_fd, int output_fd);
int								ft_export(t_data *data);
void							sort(char **tmp);
t_env							*init_export_list(char **env);
int								ft_unset(t_data *data);
char							*find_cmd_path(char *cmd, t_data *data);

// pipe
bool							is_builtin(char *cmd);
int								run_builtin(t_data *data, t_cmd *cmd);
void							exec_child(t_data *data, t_cmd *cmd,
									int prev_fd);
int								resolve_command_path(t_data *data, t_cmd *cmd);
int								ft_process(t_data *data, t_cmd *cmd,
									int prev_fd);

// ft_pipe1
void							ft_exit_exec(int code, t_data *data,
									t_cmd *cmd);
int								open_infile(char *str);
void							handle_direct_exec(t_data *data, t_cmd *cmd);
void							handle_path_exec(t_data *data, t_cmd *cmd);
void							handle_invalid_command(t_data *data, t_cmd *cmd,
									int prev_fd);

// ft_pipe2
void							safe_close(int fd);
int								redirect_management(t_cmd *cmd, int prev_fd);
void							exit_d(t_data *data);

// pipe utils
int								open_infile(char *str);
int								last_infile(t_cmd *cmd);
int								manag_infile(t_cmd *cmd, int prev_fd);
int								open_outfile(char *file, t_TokenType mode);
int								last_outfile(t_cmd *cmd);
int								manag_outfile(t_cmd *cmd, int *pipe_fd);

// error message
void							command_not_found(char *cmd);
void							no_such_file_or_directory(char *cmd);
void							permission_denied(char *file);
void							error_message(char *str);
void							is_a_directory(char *str);
int								set_fd_cloexec(int fd);

// heredoc
void							made_new_file(int *fd, char **name);
int								fill_here_doc_file(int fd, char *delimitor);
char							*get_here_doc(char *str);
void							handle_sigint(int sig);
void							reset_signals_child(void);
void							setup_signal_heredoc(void);


//term
// void save_terminal_settings(void);
// void restore_terminal_settings(void);
// void set_terminal_non_canonical(void);


#endif
