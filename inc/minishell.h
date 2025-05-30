/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:34:33 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/30 12:36:50 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE // for signals: struct sigaction, SA_RESTART
# include "../libft/libft.h"
# include <ctype.h>   // For character handling functions (if needed)
# include <dirent.h>  // For opendir, readdir, closedir
# include <errno.h>   // For error handling (if needed)
# include <fcntl.h>   // For open flags
# include <ncurses.h> // For terminal handling (if needed, e.g., tgetent,tgetstr)
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>    // For signal, sigaction
# include <stdio.h>     // For printf, perror
# include <stdlib.h>    // For malloc, free, getenv, exit
# include <string.h>    // For strerror
# include <sys/ioctl.h> // For ioctl
# include <sys/stat.h>  // For stat, lstat, fstat, unlink
# include <sys/types.h> // For data types used in system calls
# include <sys/wait.h>  // For wait, waitpid, wait3, wait4
# include <termios.h>   // For tcsetattr, tcgetattr
# include <unistd.h>    // For access, open, read, close, fork, pipe, dup, dup2,isatty, ttyname, ttyslot, execve, write

// Our project's single Global variable
extern volatile sig_atomic_t	g_signal_flag; // Tracks signal events

// minishell prototype, add on as we know
typedef struct s_minishell
{
	// char	**env; // Array of environment variables (similar to environ)
	struct s_env	*env_list;
	int stdio_backup[2];
	int		last_exit_code; // Stores the exit code of the last executed command ($?)
	char	cwd[PATH_MAX]; // Current working directory
	int		interactive; // Boolean: 1 for interactive mode,0 for non-interactive
	struct s_token *tokens;
	struct s_ast	*ast;
}						t_minishell;

// prototype, add on as we know
typedef enum e_token_type
{
	T_STRING,
	T_HEREDOC,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_PIPE,
}						t_token_type;

// consider using doubly linked list for token management
typedef struct s_token
{
	char				*value;
	t_token_type		type;
	bool	adjacent;
	struct s_token		*previous;
	struct s_token		*next;
}						t_token;

typedef struct s_exp_context 
{
	t_minishell	*shell;
	int			*in_squote;
	int			*in_dquote;
}	t_exp_context;

// struct for the ast node
typedef enum e_ast_type
{
	AST_CMD,
	AST_PIPE,
	AST_REDIR_IN,
	AST_REDIR_OUT,
	AST_APPEND,
	AST_HEREDOC,
}						t_ast_type;

// struct for the ast tree
typedef struct s_ast
{
	t_ast_type		type;
	int	heredoc_fd;
	bool	hd_quoted;
	char			**args;
	char			*file; // For redirections
	struct s_ast	*left;
	struct s_ast	*right;
	t_minishell		*shell;
}						t_ast;

// prototype fordoing heredoc
typedef struct s_heredoc
{
	char				*delimiter;
	bool	hd_quoted;
	// int					pipefd[2];
	t_ast				*node; // this is from kagoh AST
	struct s_heredoc	*next;
}						t_heredoc;

// Struct for pipe child process configuration
typedef struct s_pipechild
{
	t_ast		*ast;
	t_minishell	*minishell;
	int			read_fd;
	int			write_fd;
	int			std_fd;
}						t_pipechild;

// Structure for storing key-value pairs of environment variables
typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

/* function prototypes */

// lexing functions
int				check_pipe(t_token *head);
int				check_redirects(t_token *token);
int				check_heredoc(t_token *head);
int				check_grammar(t_token *head);
int				check_quotes(t_token *head);
void			toggle_quote_state(char c, int *in_dquote,
					int *in_squote);
void			free_split(char **split);
t_token	*create_token(char *value, t_token_type type);
// void	free_tokens(t_token **head);
void	free_tokens(t_token *head);
bool	is_metachar(char c);
bool	is_heredoc(char *input, int i);
bool	is_append(char *input, int i);
t_token	*tokenize_hd(char *input, int *i);
t_token	*tokenize_append(char *input, int *i);
t_token	*tokenize_metachar(char *input, int *i);
char	*extract_string(char *input, int *i);
t_token	*tokenize_string(char *input, int *i);
t_token	*tokenize(char *input);
int ft_isspace(int c);
bool	is_redirection(t_token *token);
bool	is_invalid_redirection_sequence(char *input, int i);
char	*remove_quotes(char *str);
void	expand_variables(t_token *token, t_minishell *shell);
t_token	*handle_redirection_expansion(t_token *token, t_minishell *shell);
t_token	*expand_all_tokens(t_token *tokens, t_minishell *shell);
t_token *concatenate_adjacent_strings(t_token *tokens);

char	*remove_outer_quotes(char *str);
char *expand_variables_with_quotes(char *str, t_minishell *shell);
int	is_valid_pipe_side(t_token *token, int direction);
t_token	*classify_token(char *input, int *i);
void	link_tokens(t_token **head, t_token **current, t_token *new_token);
int	process_char(char c, int *in_squote, int *in_dquote, char **result);

void	append_char(char **result, char *ch);
void	append_str(char **result, char *str);
void	process_expansion_char(char *str, int *i, char **result, t_exp_context ctx);
void	handle_single_quote(int *i, char **result, int *in_squote);
void	handle_double_quote(int *i, char **result, int *in_dquote);
void	handle_variable_expansion(char *str, int *i, char **result, t_minishell *shell);
char	*extract_variable_name(char *str, int *i);
char	*get_variable_value(t_minishell *shell, char *var_name);
// void	append_char_to_result(char **result, char c);
// void	handle_exit_status(char **result, t_minishell *shell);
// void	handle_variable_name(char *str, size_t *i, char **result, t_minishell *shell);
// void	handle_variable_expansion_in_str(char *str, size_t *i, char **result, t_minishell *shell);

t_token	*extract_token(char *input, int *i, t_token **head, t_token **current);
// parsing functions
t_ast	*parse_pipeline(t_token **tokens, t_minishell *shell);
t_ast	*parse_command(t_token **tokens, t_minishell *shell);
char	**parse_arguments(t_token **tokens);
t_ast	*parse_redirection(t_token **tokens, t_ast *left, t_minishell *shell);
size_t	count_arguments(t_token *tokens);
t_ast	*parse_heredoc(t_token **tokens, t_ast *left, t_minishell *shell);
void	free_ast(t_ast *node);
t_ast	*create_ast_node(t_ast_type type, t_minishell *shell);
t_ast_type classify_redir(t_token_type token_type);
void	process_heredocs(t_ast *ast, t_minishell *shell);
void	process_heredoc_input(t_heredoc *hd, t_minishell *shell);
// int	create_heredoc_pipe(t_heredoc *hd);
void close_unused_heredocs(t_ast *root, t_ast *current_node);
void	ft_strcat(char *dst, const char *src);
void	ft_strcpy(char *dst, const char *src);
void	ft_itoa_into(char *buf, int n);

// int	create_heredoc_tempfile(t_minishell *shell);
int	create_heredoc_tempfile(char *out_path);
int	handle_heredoc_logic(t_heredoc *hd, t_minishell *shell, char *line);
char	**append_arg(char **args, char *new_arg);
void	expand_heredoc_line(t_token *token, t_minishell *shell);

// env functions
t_minishell		*init_minishell(char **envp);
void			init_fields(t_minishell *minishell, char **envp);
void			free_minishell(t_minishell *minishell);
t_env	*new_env_node(const char *key, const char *value);
void	add_env_var(t_env **env, const char *key, const char *value);
t_env	*find_env_var(t_env *env, const char *key);
void	update_env_var(t_env *env, const char *key, const char *new_value);
t_env	*init_env(char **envp);
void	update_shlvl(t_env *env);
void	print_env_vars(t_env *env);
void	delete_env_var(t_env **env, const char *key);
void	free_env(t_env *env);
int	is_valid_env_name(const char *str);
void	print_export_var(t_env *var, int fd_out);
char	**env_to_keys_array(t_env *env);
void	print_sorted_env(t_env *env, int fd_out);
void	swap_strings(char **a, char **b);
void	sort_keys(char **tab);
char	*ft_strndup(const char *s, size_t n);

t_env	*init_minimal_env(void);

// builtin functions
int	update_pwds(t_minishell *shell, t_env **env_list, char *new_path);
int	change_directory(t_minishell *shell, t_env **env_list, char *path);
char	*expand_path(t_minishell *shell, t_env **env_list, char *path);
int	b_cd(t_minishell *shell, t_env **env_list, char **args);
void    print_args(char **args, int nflag);
int b_echo(char **args);
int b_env(t_env *env_list);
int	is_valid_exit_arg(char *arg);
void	cleanup_and_exit(t_minishell *shell, int exit_code);
int	b_exit(t_minishell *shell, char **args);
int b_pwd(void);
int b_unset(t_env *env, char **args);
int	process_export_arg(t_env **env, char *arg);
int	bi_export(t_minishell *shell, char **args, int fd_out);

bool	is_empty_assignment(const char *arg);
// int handle_overflow(const char *str);
long long	ft_atoll(const char *str);
// long long handle_overflow(const char *str);
int	invalid_identifier(char *arg);
int	have_equals(t_env **env, char *arg, char *equal_sign);
// signals
void    sig_interactive(void);
void    handle_signal(int sig_num);
void handle_heredoc_sigint(int sig);
void setup_heredoc_signals(void);
void    sig_reset(bool for_child);
int	rl_heredoc(void);
// void    sig_ignore(void);
// void    sig_cmd(int sig_num);

// execution functions
int	execute_builtin(t_minishell *shell, char **args, int fd_out);
// void	execute_external(t_ast *node, t_minishell *shell);
int execute_external(t_ast *node, t_minishell *shell);
char	*find_command_path(char *cmd, t_minishell *shell);
void	execute_command(t_ast *node, t_minishell *shell);
// void error_command_not_found(char *cmd);
// void	handle_exec_error(char *cmd, char *path, char **env_array);
void	cmd_not_found(char *cmd);
void	permission_denied(char *cmd);
void	no_file(char *cmd);
void	path_not_set(char *cmd);

// void execute_pipeline(t_ast *node, t_minishell *shell);
int	execute_redirection(t_ast *node, t_minishell *shell);
char	**convert_env_to_array(t_env *env_list);
char	*join_str(char const *s1, char const *s2, char const *s3);
int setup_redirections(t_ast *node, t_minishell *shell);
int is_builtin(char *cmd);
void handle_parent_process(pid_t pid, t_minishell *shell);
int execution_logic(t_ast *ast, t_minishell *minishell);
void	restore_standard_fds(t_minishell *shell);

void	execute_pipeline(t_ast *node, t_minishell *shell, int input_fd);
void	backup_fds(t_minishell *shell);
// void execute_pipe_segment(t_pipechild *pc);
// void wait_for_children(t_minishell *shell);
// char	*check_direct_path(char *cmd);
char	*check_direct_path(char *cmd, t_minishell *shell);
// char	*search_in_path(char **dirs, char *cmd);
char	*search_in_path(char **dirs, char *cmd, t_minishell *shell);
void	not_bi(t_ast *node, t_minishell *shell);
int	get_redirection_info(t_ast *node, int *flags, int *original_fd);
pid_t	exec_left(t_ast *node, t_minishell *shell, int input_fd, int pipe_fd[2]);
pid_t	exec_right(t_ast *node, t_minishell *shell, int pipe_fd[2]);
// void	go_to_sleep(pid_t l_pid, pid_t r_pid, t_minishell *shell);
void	go_to_sleep(pid_t last_pid, t_minishell *shell);
int	count_env(t_env *env_list);

//main functions
char	*pre_token(int *exit_status, t_minishell *shell);
int	lex_and_expand(t_minishell *shell, char *input);
int	parse_and_exec(t_minishell *shell, int *exit_status);
// int ft_strcmp(char *s1, char *s2);
int	ft_strcmp(const char *s1, const char *s2);

#endif
