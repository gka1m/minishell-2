/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:08:58 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/23 16:14:33 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// void	execute_pipeline(t_ast *node, t_minishell *shell)
// {
// 	int		pipe_fd[2];
// 	pid_t	pid;
// 	int		left_fd;
// 	int		status;

// 	left_fd = STDIN_FILENO;
// 	if (!node || node->type != AST_PIPE)
// 		return ;
// 	// Parent ignores signals during pipeline setup
// 	sig_reset(false);
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("minishell: pipe");
// 		shell->last_exit_code = 1;
// 		sig_interactive();
// 		return ;
// 	}
// 	// Left command (writes to pipe)
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		// Child process setup
// 		sig_reset(true);
// 		close(pipe_fd[0]); // Close unused read end
// 		// Setup redirections (including heredoc if present)
// 		// Connect stdout to pipe
// 		dup2(pipe_fd[1], STDOUT_FILENO);
// 		close(pipe_fd[1]);
// 		if (setup_redirections(node->left, shell) == -1)
// 			exit(1);
// 		execute_command(node->left, shell);
// 		exit(shell->last_exit_code);
// 	}
// 	else if (pid < 0)
// 	{
// 		perror("minishell: fork");
// 		close(pipe_fd[0]);
// 		close(pipe_fd[1]);
// 		shell->last_exit_code = 1;
// 		sig_interactive();
// 		return ;
// 	}
// 	// Right side preparation
// 	close(pipe_fd[1]);    // Close write end in parent
// 	left_fd = pipe_fd[0]; // Save read end for next command
// 	// Handle right side
// 	if (node->right->type == AST_PIPE)
// 	{
// 		execute_pipeline(node->right, shell);
// 	}
// 	else
// 	{
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			// Child process setup
// 			sig_reset(true);
// 			// Connect stdin to pipe
// 			dup2(left_fd, STDIN_FILENO);
// 			close(left_fd);
// 			// Setup other redirections
// 			if (setup_redirections(node->right, shell) == -1)
// 				exit(1);
// 			execute_command(node->right, shell);
// 			exit(shell->last_exit_code);
// 		}
// 		else if (pid < 0)
// 		{
// 			perror("minishell: fork");
// 			close(left_fd);
// 			shell->last_exit_code = 1;
// 			sig_interactive();
// 			return ;
// 		}
// 	}
// 	// Cleanup in parent
// 	close(left_fd);
// 	sig_interactive();
// 	// Wait for pipeline completion
// 	while (wait(&status) > 0)
// 	{
// 		if (WIFEXITED(status))
// 			shell->last_exit_code = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 		{
// 			shell->last_exit_code = 128 + WTERMSIG(status);
// 			if (WTERMSIG(status) == SIGINT)
// 				ft_putstr_fd("\n", STDERR_FILENO);
// 		}
// 	}
// }

void	execute_pipeline(t_ast *node, t_minishell *shell)
{
	int pipe_fd[2];
	pid_t pid;
	int left_fd;
	int status;

	if (!node || node->type != AST_PIPE)
		return ;

	sig_reset(false);

	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		shell->last_exit_code = 1;
		// sig_interactive();
		return ;
	}

	// Left command (writes to pipe)
	pid = fork();
	if (pid == 0)
	{
		sig_reset(true);
		close(pipe_fd[0]);

		// Redirect stdout to pipe
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(pipe_fd[1]);

		if (setup_redirections(node->left, shell) == -1)
			exit(1);

		execute_command(node->left, shell);
		exit(shell->last_exit_code);
	}
	else if (pid < 0)
	{
		perror("minishell: fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		shell->last_exit_code = 1;
		// sig_interactive();
		return ;
	}

	// Right side preparation
	close(pipe_fd[1]);
	left_fd = pipe_fd[0];

	// Handle right side
	if (node->right->type == AST_PIPE)
	{
		execute_pipeline(node->right, shell);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			sig_reset(true);

			// Redirect stdin from pipe
			if (dup2(left_fd, STDIN_FILENO) == -1)
			{
				perror("minishell: dup2");
				exit(1);
			}
			close(left_fd);

			if (setup_redirections(node->right, shell) == -1)
				exit(1);

			execute_command(node->right, shell);
			exit(shell->last_exit_code);
		}
		else if (pid < 0)
		{
			perror("minishell: fork");
			close(left_fd);
			shell->last_exit_code = 1;
			// sig_interactive();
			return ;
		}
	}

	// Cleanup in parent
	close(left_fd);
	

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
	restore_standard_fds(shell);
	sig_interactive();
}