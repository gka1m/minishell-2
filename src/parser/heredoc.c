/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:24:32 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/07 17:26:28 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	create_heredoc_pipe(t_heredoc *hd)
{
	if (pipe(hd->pipefd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	return (1);
}

// void	process_heredoc_input(t_heredoc *hd, t_minishell *shell)
// {
// 	char	*line;
// 	t_token	*temp_token;

// 	setup_heredoc_signals();
// 	while (!g_signal_flag)
// 	{
// 		line = readline("> ");
// 		if (g_signal_flag)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (!line)
// 		{
// 			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n",
// 				STDERR_FILENO);
// 			break ;
// 		}
// 		if (hd->delimiter && strcmp(line, hd->delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (!hd->hd_quoted)
// 		{
// 			temp_token = create_token(line, T_STRING);
// 			if (temp_token)
// 			{
// 				expand_variables(temp_token, shell);
// 				write(hd->pipefd[1], temp_token->value,
// 					ft_strlen(temp_token->value));
// 				write(hd->pipefd[1], "\n", 1);
// 				free_tokens(&temp_token);
// 			}
// 		}
// 		else
// 		{
// 			write(hd->pipefd[1], line, ft_strlen(line));
// 			write(hd->pipefd[1], "\n", 1);
// 		}
// 		free(line);
// 	}
// 	close(hd->pipefd[1]);
// }

void	process_heredoc_input(t_heredoc *hd, t_minishell *shell)
{
	char	*line;
	t_token	*temp_token = NULL;

	setup_heredoc_signals();
	while (!g_signal_flag)
	{
		line = readline("> ");
		if (g_signal_flag)
		{
			free(line);
			if (temp_token)
				free_tokens(temp_token);
			close(hd->pipefd[1]);
			// break ;
			exit(130);
		}
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n",
				STDERR_FILENO);
			// g_signal_flag = 1;
			break ;
		}
		if (strcmp(line, hd->delimiter) == 0)
		{
			free(line);
			break ;
		}
		// Write to pipe only
		if (!hd->hd_quoted)
		{
			temp_token = create_token(line, T_STRING);
			if (temp_token)
			{
				expand_variables(temp_token, shell);
				write(hd->pipefd[1], temp_token->value,
					ft_strlen(temp_token->value));
				write(hd->pipefd[1], "\n", 1);
				free_tokens(temp_token);
			}
		}
		else
		{
			write(hd->pipefd[1], line, ft_strlen(line));
			write(hd->pipefd[1], "\n", 1);
		}
		free(line);
	}
	close(hd->pipefd[1]);
	if (g_signal_flag)
	{
		free(line);
		if (temp_token)
			free_tokens(temp_token);
		free_ast(hd->node);
		exit(130);
	}
	// exit(0);
	// rl_replace_line("", 0);
	// rl_redisplay();
}

// void	process_heredocs(t_ast *ast, t_minishell *shell)
// {
// 	t_heredoc	hd;

// 	if (!ast)
// 		return ;
// 	if (ast->type == AST_HEREDOC)
// 	{
// 		hd.delimiter = ast->file;
// 		hd.hd_quoted = ast->hd_quoted;
// 		if (!create_heredoc_pipe(&hd))
// 			return ;
// 		process_heredoc_input(&hd, shell);
// 		ast->heredoc_fd = hd.pipefd[0]; // Store only the read end
// 		// Write end is already closed by process_heredoc_input
// 		if (g_signal_flag)
// 			shell->last_exit_code = 130;
// 	}
// 	process_heredocs(ast->left, shell);
// 	process_heredocs(ast->right, shell);
// }

// void	process_heredocs(t_ast *ast, t_minishell *shell)
// {
// 	t_heredoc	hd;
// 	pid_t		pid;
// 	int			status;

// 	if (!ast)
// 		return;

// 	if (ast->type == AST_HEREDOC)
// 	{
// 		hd.delimiter = ast->file;
// 		hd.hd_quoted = ast->hd_quoted;
// 		if (!create_heredoc_pipe(&hd))
// 			return;

// 		pid = fork();
// 		if (pid == 0)
// 			process_heredoc_input(&hd, shell); // child

// 		// parent
// 		close(hd.pipefd[1]); // close write end immediately
// 		waitpid(pid, &status, 0);

// 		if (WIFEXITED(status))
// 		{
// 			int exit_code = WEXITSTATUS(status);
// 			if (exit_code != 0) // interrupted
// 			{
// 				close(hd.pipefd[0]);
// 				g_signal_flag = 1;
// 				shell->last_exit_code = 130;
// 				return;
// 			}
// 			ast->heredoc_fd = hd.pipefd[0]; // valid heredoc
// 		}
// 		else if (WIFSIGNALED(status))
// 		{
// 			close(hd.pipefd[0]);
// 			g_signal_flag = 1;
// 			shell->last_exit_code = 128 + WTERMSIG(status);
// 			return;
// 		}
// 	}

// 	process_heredocs(ast->left, shell);
// 	process_heredocs(ast->right, shell);
// }

void process_heredocs(t_ast *ast, t_minishell *shell)
{
    t_heredoc hd;
    pid_t pid;
    int status;

    if (!ast)
		return;
    if (ast->type == AST_HEREDOC)
    {
        hd.delimiter = ast->file;
        hd.hd_quoted = ast->hd_quoted;
        if (!create_heredoc_pipe(&hd))
            return;
        pid = fork();
        if (pid == 0) {
            // Child process - collect heredoc content
            setup_heredoc_signals();
            process_heredoc_input(&hd, shell);
            exit(0);
        } else {
            // Parent process
            close(hd.pipefd[1]); // Close write end
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ast->heredoc_fd = hd.pipefd[0]; // Store read end
                
                // // If this is a simple command (not pipeline), execute immediately
                // if (ast->right == NULL && ast->left == NULL) {
                //     execute_command(ast, shell);
                //     close(hd.pipefd[0]);
                //     return;
                // }
            } else {
                close(hd.pipefd[0]);
                if (WIFSIGNALED(status)) {
                    shell->last_exit_code = 128 + WTERMSIG(status);
                }
				return ;
            }
        }
    }
    process_heredocs(ast->left, shell);
    process_heredocs(ast->right, shell);
}