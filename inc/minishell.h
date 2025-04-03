/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:34:33 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/03 16:28:44 by kagoh            ###   ########.fr       */
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
	char	**env; // Array of environment variables (similar to environ)
	struct s_env	*env_list;
	int		last_exit_code; // Stores the exit code of the last executed command ($?)
	char	cwd[PATH_MAX]; // Current working directory
	int		interactive; // Boolean: 1 for interactive mode,0 for non-interactive
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
	T_EOF,
}						t_token_type;

// consider using doubly linked list for token management
typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*previous;
	struct s_token		*next;
}						t_token;

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
	char			**args;
	char			*file; // For redirections
	struct s_ast	*left;
	struct s_ast	*right;
	t_minishell		*shell;
}						t_ast;

typedef struct s_parse_ctx
{
	char		*input;
	int			*i;
	t_minishell	*shell;
	char		**result;
}						t_parse_ctx;

// prototype fordoing heredoc
typedef struct s_heredoc
{
	char				*delimiter;
	int					pipefd[2];
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
int				check_redirects(t_token *head);
int				check_append(t_token *head);
int				check_heredoc(t_token *head);
int				check_grammar(t_token *head);
int				check_eof(t_token *head);
int				check_quotes(t_token *head);
void			toggle_quote_state(char c, int *in_dquote,
					int *in_squote);
t_token			*create_token(char *value, t_token_type type);
void			add_token(t_token **head, t_token *new);
t_token_type	classify_token(char *input);
void			free_tokens(t_token **head);
t_token			*tokenize_input(char **split);
void			free_split(char **split);
int				is_quote(char c);
int				get_quoted_len(const char *s);
int				count_words(const char *s, char c);
char			*get_next_word(const char *s, char c, int *i);
char			**split_preserve_quotes(const char *s, char c);

void			concatenate_quoted_strings(t_token *last, t_token *new);
int				should_concatenate(t_token *last, char *current_str);

// parsing functions
t_ast			*create_ast_node(t_ast_type type, t_minishell *shell);
t_ast			*parse_pipeline(t_token **tokens, t_minishell *shell);
t_ast			*parse_redir(t_token **tokens, t_minishell *shell);
t_ast			*parse_hd(t_token **tokens, t_minishell *shell);
void			parse_arguments(t_token **tokens, t_ast *cmd_node);
t_ast			*parse_command(t_token **tokens, t_minishell *shell);
void			print_ast(t_ast *node, int level);
void			free_ast(t_ast *node);
void			add_argument(char ***args, char *value);

int				is_valid_var_char(char c, int first_char);
char			*extract_var_name(char *input, int *i);
char			*handle_question_var(t_minishell *shell);
char			*get_var_value(char *var_name, t_minishell *shell);
char			*ft_strjoin_free(char *s1, char *s2);
char			*ft_strjoin_char(char *s1, char c);
int				handle_squote(char *input, int *i, char **result);
int				handle_dquote(char *input, int *i, t_minishell *shell,
					char **result);
int				expand_var(char *input, int *i, t_minishell *shell,
					char **result);
char			*expand_input(char *input, t_minishell *shell);
int				handle_dquote_expansion(t_parse_ctx *ctx, int *start);
int				handle_specials(char *input, int *i, t_minishell *shell,
					char **result);

void	expand_squote_redir(char **input, int *i, char **result);
void	expand_dquote_redir(char **input, int *i, t_minishell *shell,
	char **result);
char	*expand_redirection_target(char *input, t_minishell *shell);
// void	process_dquote_content(t_parse_ctx *ctx);
// void	redir_dquote(t_parse_ctx *ctx);
// void	process_squote_content(t_parse_ctx *ctx);
// void	redir_squote(t_parse_ctx *ctx);
// void	handle_var_expansion(t_parse_ctx *ctx);
// char	*expand_redir(char *input, t_minishell *shell, bool heredoc_mode);

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

// signals
void    sig_interactive(void);
void    handle_sigint(int sig_num);
void    sig_reset(bool for_child);
void    sig_ignore(void);
void    sig_cmd(int sig_num);
// void    sigint_handler(int signo);
// void    sigint_heredoc(int signo);
// int rl_event_hd(void);
// void	setup_sig_interactive(void);
// void	setup_sig_exec(void);
// void	setup_sig_heredoc(void);
// int process_input(char **input);


// char	*join_str(char const *s1, char const *s2, char const *s3);
// char	**convert_env_to_array(t_env *env_list);

#endif
