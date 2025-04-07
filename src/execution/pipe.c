/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:08:58 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/07 14:02:09 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_pipeline(t_ast *node, t_minishell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		left_fd;
	int		status;

	left_fd = STDIN_FILENO;
	if (!node || node->type != AST_PIPE)
		return ;
	// Parent ignores signals during pipeline setup
	sig_ignore();
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		shell->last_exit_code = 1;
		sig_interactive(); // Restore signals on error
		return ;
	}
	// Left command (writes to pipe)
	pid = fork();
	if (pid == 0)
	{
		// Child process setup
		sig_reset(true); // Default handlers with SA_RESTART
		close(pipe_fd[0]);
		if (left_fd != STDIN_FILENO)
		{
			dup2(left_fd, STDIN_FILENO);
			close(left_fd);
		}
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		execute_command(node->left, shell);
		exit(shell->last_exit_code);
	}
	else if (pid < 0)
	{
		perror("minishell: fork");
		// close_pipe(pipe_fd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		shell->last_exit_code = 1;
		sig_interactive(); // Restore signals
		return ;
	}
	// Right side preparation
	close(pipe_fd[1]);
	left_fd = pipe_fd[0];
	// Handle right side (recursive for multiple pipes)
	if (node->right->type == AST_PIPE)
		execute_pipeline(node->right, shell);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			// Last command in pipeline
			sig_reset(true); // Default handlers
			dup2(left_fd, STDIN_FILENO);
			if (left_fd != STDIN_FILENO)
				close(left_fd);
			execute_command(node->right, shell);
			exit(shell->last_exit_code);
		}
		else if (pid < 0)
		{
			perror("minishell: fork");
			close(left_fd);
			shell->last_exit_code = 1;
			sig_interactive(); // Restore signals
			return ;
		}
	}
	// Cleanup in parent
	close(left_fd);
	// Restore interactive signals before waiting
	sig_interactive();
	// Wait for pipeline completion
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			shell->last_exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			shell->last_exit_code = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				ft_putstr_fd("\n", STDERR_FILENO);
		}
	}
}
