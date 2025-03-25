/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:34:33 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/25 14:00:15 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE  // for signals: struct sigaction, SA_RESTART
# include <ctype.h>   // For character handling functions (if needed)
# include <dirent.h>  // For opendir, readdir, closedir
# include <errno.h>   // For error handling (if needed)
# include <fcntl.h>   // For open flags
# include <ncurses.h> // For terminal handling (if needed, e.g., tgetent, tgetstr)
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
# include <unistd.h>    // For access, open, read, close, fork, pipe, dup, dup2, isatty, ttyname, ttyslot, execve, write
# include "../libft/libft.h"

// Our project's single Global variable
extern volatile sig_atomic_t	g_signal_flag; // Tracks signal events

// minishell prototype, add on as we know
typedef struct s_minishell
{
	char **env;         // Array of environment variables (similar to environ)
	int last_exit_code; // Stores the exit code of the last executed command ($?)
	char cwd[PATH_MAX]; // Current working directory
	int interactive;    // Boolean: 1 for interactive mode, 0 for non-interactive
}	t_minishell;

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
	t_ast_type			type;
	char				**args;
	char *file; // For redirections
	struct s_ast		*left;
	struct s_ast		*right;
	t_minishell			*shell;
}						t_ast;

// prototype fordoing heredoc
typedef struct s_heredoc
{
	char				*delimiter;
	int					pipefd[2];
	t_ast				*node;  //this is from kagoh AST
	struct s_heredoc	*next;
}						t_heredoc;

// Struct for pipe child process configuration
typedef struct  s_pipechild
{
    t_ast		*ast;
    t_minishell	*minishell;
    int			read_fd;
    int			write_fd;
    int			std_fd;
}				t_pipechild;

// Structure for storing key-value pairs of environment variables
typedef struct s_env
{
    char			*key;
    char			*value;
    struct s_env	*next;
}					t_env;

/* function prototypes */

// lexing functions
int	check_pipe(t_token *head);
int	check_redirects(t_token *head);
int	check_append(t_token *head);
int	check_heredoc(t_token *head);
int	check_grammar(t_token *head);
int	check_eof(t_token *head);
int	check_quotes(t_token *head);
void	toggle_quote_state(char c, int *in_dquote, int *in_squote);
t_token					*create_token(char *value, t_token_type type);
void					add_token(t_token **head, t_token *new);
t_token_type			classify_token(char *input);
void					free_tokens(t_token **head);
t_token					*tokenize_input(char **split);
void					free_split(char **split);
int	is_quote(char c);
int	get_quoted_len(const char *s);
int	count_words(const char *s, char c);
char	*get_next_word(const char *s, char c, int *i);
char	**split_preserve_quotes(const char *s, char c);

// parsing functions
t_ast	*create_ast_node(t_ast_type type, t_minishell *shell);
t_ast   *parse_pipeline(t_token **tokens, t_minishell *shell);
t_ast	*parse_redir(t_token **tokens, t_minishell *shell);
t_ast	*parse_hd(t_token **tokens, t_minishell *shell);
void	parse_arguments(t_token **tokens, t_ast *cmd_node);
t_ast	*parse_command(t_token **tokens, t_minishell *shell);
void	print_ast(t_ast *node, int level);
void	free_ast(t_ast *node);
void	add_argument(char ***args, char *value);


// signals
// void    sigint_handler(int signo);
// void    sigint_heredoc(int signo);
// int rl_event_hd(void);
// void	setup_sig_interactive(void);
// void	setup_sig_exec(void);
// void	setup_sig_heredoc(void);
// int process_input(char **input);

// env functions
t_minishell	*init_minishell(char **envp);
void	init_fields(t_minishell *minishell, char **envp);
void	free_minishell(t_minishell *minishell);

void	print_env_vars(char **env);

#endif

