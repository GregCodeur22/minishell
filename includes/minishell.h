/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:20:24 by garside           #+#    #+#             */
/*   Updated: 2025/06/13 14:01:09 by garside          ###   ########.fr       */
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
	int							quoted;
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

//-------------Exec---------------
// find_cmd_path.c
void							free_split(char **tmp);
char							*try_paths(char **paths, char *cmd);
char							*find_cmd_path(char *cmd, t_data *data);

// ft_exec_utils_1.c
void							free_data(t_data *data);
char							**ft_get_cmd(t_data *data);
void							ft_replace_in_env(t_data *data, char *name,
									char *value);

// ft_exec.c
int								handle_single_command(t_data *data, t_cmd *cmd,
									int prev_fd);
void							handle_useless_command(t_cmd *cmd,
									int *prev_fd);
int								wait_for_children(t_data *data, pid_t last_pid,
									int prev_fd);
int								exec_line(t_data *data, t_cmd *cmd);

// ft_exec1.c
char							*get_cmd_path(t_data *data, char **cmd);
void							is_not_path(t_data *data, char **args,
									char *path);
int								exec_child_process(t_data *data, t_cmd *cmd,
									int prev_fd);
int								ft_shell(t_data *data, t_cmd *cmd, int prev_fd);
int								which_command(t_data *data, t_cmd *cmd,
									int prev_fd);

// ft_executables.c
void							exit_clean(t_data *data);
void							exit_clean2(t_data *data, t_cmd *cmd);
void							redire_exec(t_data *data, t_cmd *cmd,
									int input_fd, int output_fd);
int								ft_executables(t_data *data, t_cmd *cmd,
									int input_fd, int output_fd);
// ft_pipe_error.c
void							command_not_found(char *cmd);
void							no_such_file_or_directory(char *cmd);
void							permission_denied(char *file);
void							error_message(char *str);
void							is_a_directory(char *str);
// ft_pipe_utils.c
int								last_infile(t_cmd *cmd);
int								manag_infile(t_cmd *cmd, int prev_fd);
int								open_outfile(char *file, t_TokenType mode);
int								last_outfile(t_cmd *cmd);
int								manag_outfile(t_cmd *cmd, int *pipe_fd);
// ft_pipe.c
bool							is_builtin(char *cmd);
int								run_builtin(t_data *data, t_cmd *cmd);
void							exec_child(t_data *data, t_cmd *cmd,
									int prev_fd);

// ft_pipe1.c
void							ft_exit_exec(int code, t_data *data,
									t_cmd *cmd);
int								open_infile(char *str);
int								handle_direct_exec(t_data *data, t_cmd *cmd,
									int prev_fd);
int								handle_path_exec(t_data *data, t_cmd *cmd);
void							handle_invalid_command(t_data *data, t_cmd *cmd,
									int prev_fd);
// ft_pipe2.c
void							safe_close(int fd);
int								redirect_management(t_cmd *cmd, int prev_fd);
void							exit_d(t_data *data);

// ft_pipe3.c
int								empty_line(const char *str);
int								resolve_command_path(t_data *data, t_cmd *cmd);
int								ft_process(t_data *data, t_cmd *cmd,
									int prev_fd);
void							handle_failed_resolution(t_data *data,
									t_cmd *cmd, int prev_fd);
void							handle_failed_permission(t_data *data,
									t_cmd *cmd, int prev_fd);

//-------------Builtins---------------
// ft_cd.c
int								update_pwd_env(t_data *data);
int								update_oldpwd_and_chdir(t_data *data,
									char *path);
int								cd_to_home(t_data *data);
int								cd_to_path(t_data *data, char *path);
int								ft_cd(t_data *data);

// ft_echo.c
int								ft_echo(t_data *data, t_cmd *cmd);

// ft_env.c
int								ft_env(t_data *data);

// ft_exit.c
int								ft_isalldigit(char *str);
int								ft_exit(t_data *data, t_cmd *cmd, int stdin,
									int stdout);

// ft_export_utils_1.c
int								ft_is_valid(char *str);
int								check_name(char *str, t_env *node,
									char *content);
char							*get_content(char *str, int i);

// ft_export.c
void							handle_existing_name(t_data *data, char *name,
									char *content);
int								add_in_export(t_data *data, char *str);
int								export_variable(t_data *data, t_cmd *cmd);
int								ft_export(t_data *data);

// ft_pwd.c
int								ft_pwd(void);

// ft_unset.c
void							remove_from_list(t_env **list, char *name);
int								ft_unset(t_data *data);
void							exit_cmd_not_found(t_data *data, t_cmd *cmd,
									char *arg);
void							print_cmd_error(char *cmd);

//-------------Parsing---------------
// ft_here_doc.c
void							made_new_file(int *fd, char **name);
int								if_limiter(char *line, char *limiter);
void							free_path(char *path_cmd);
int								fill_here_doc_file(int fd, char *delimitor);
char							*get_here_doc(char *str);

// ft_lexer_utils_1.c
void							free_one_token(t_token *token);
void							free_token(t_token *head);
t_token							*new_token(char *value, t_TokenType type,
									int quoted);
char							*handle_error_code(t_data *data, char *value,
									int *i);
void							add_token_to_list(t_token **head,
									t_token **last, t_token *new_token);

// ft_lexer_utils_2.c
int								is_skippable_char(char c);
int								is_token_char(char c);
char							*handle_env_value(t_data *data, int *i,
									char *value);
char							*handle_plain_text(t_data *data, int *i,
									char *value);
t_token							*handle_pipe(int *i);

// ft_lexer.c
char							*handle_quotes_part(t_data *data, int *i,
									char *value);
t_token							*handle_cmd_or_arg(t_data *data, int *i);
t_token							*handle_double_redir(char *input, int *i);
t_token							*handle_redirection(char *input, int *i);
void							skip_whitespace(const char *input, int *i);

// ft_parce_util.c
int								add_redir(t_redir **redir_list, char *filename,
									int type, int *skip_next_word);
int								handle_redirection2(t_cmd *curr, t_token *token,
									int *skip_next_word);
int								create_parse(t_token *token, t_cmd **curr,
									int *skip_next_word);
int								loop_parse(t_token *token, t_cmd **curr,
									t_cmd **head, int *skip_next_word);
t_cmd							*parse_tokens(t_data *data);

// ft_parce_utils1.c
void							free_redir_list(t_redir *redir);
void							free_cmd_list(t_data *data);
t_cmd							*new_cmd_node(void);
void							free_cmd_list2(t_cmd *cmd);
void							add_arg(t_cmd *cmd, char *value);

// ft_parse.c
t_token							*get_next_token(t_data *data, int *i);
int								is_token_ok(t_token *token);
t_token							*check_token_number(t_token *current,
									char *trimmed);

// ft_parse1.c
t_env							*env_new(char *name, char *value);
void							free_env_list(t_env *new_list);

// ft_parse2.c
t_token							*ft_lexer(t_data *data);
void							print_tokens(t_data *data);
int								valid_parse(t_data *data);
int								parse(t_data *data);

// ft_quote_utils_1.c
int								check_quotes(char *input);
char							*ft_get_env(char *str, t_data *data);
char							*change_env(t_data *data, int *i);
char							*append_error_code(t_data *data, char *extract,
									int *i, int *first);

// ft_quote.c
char							*append_env_variable(t_data *data,
									char *extract, int *i, int *first);
char							*append_remaining_segment(t_data *data,
									char *extract, int first, int i);
char							*extract_word_double(t_data *data, int *i);
char							*extract_word_single(char *input, int *i);
char							*handle_quotes(t_data *data, int *i);

//-------------Lib---------------
void							ft_lstadd_back_env(t_env **lst, t_env *new);

//-------------Main---------------
t_env							*init_env_list(char **env);
void							sort(char **tmp);
t_env							*init_export_list(char **env);
void							read_prompt(t_data *data);
int								main(int ac, char **av, char **env);

//-------------Signal---------------
void							signal_handler_here_doc(int signum);
void							setup_signal_heredoc(void);
void							handle_sigint(int sig);
void							init_signal(void);
void							reset_signals_child(void);

#endif
