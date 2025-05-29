/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:08:58 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/27 17:04:38 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// void	execute_pipeline(t_ast *node, t_minishell *shell, int input_fd)
// {
// 	int	pipe_fd[2];
// 	int	status;

// 	pid_t l_pid, r_pid;
// 	if (!node || node->type != AST_PIPE)
// 		return ;
// 	sig_reset(false);
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("minishell: pipe");
// 		shell->last_exit_code = 1;
// 		return ;
// 	}
// 	l_pid = fork();
// 	if (l_pid == 0)
// 	{
// 		sig_reset(true);
// 		close(pipe_fd[0]);
// 		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
// 			exit(1);
// 		close(pipe_fd[1]);
// 		if (input_fd != -1)
// 		{
// 			if (dup2(input_fd, STDIN_FILENO) == -1)
// 				exit(1);
// 			close(input_fd);
// 		}
// 		close_unused_heredocs(shell->ast, node->left);
// 		if (setup_redirections(node->left, shell) == -1)
// 			exit(1);
// 		execute_command(node->left, shell);
// 		cleanup_and_exit(shell, shell->last_exit_code);
// 	}
// 	close(pipe_fd[1]);
// 	if (input_fd != -1)
// 		close(input_fd);
// 	if (node->right->type == AST_PIPE)
// 	{
// 		execute_pipeline(node->right, shell, pipe_fd[0]);
// 	}
// 	else
// 	{
// 		r_pid = fork();
// 		if (r_pid == 0)
// 		{
// 			sig_reset(true);
// 			close(pipe_fd[1]);
// 			if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
// 				exit(1);
// 			close(pipe_fd[0]);
// 			close_unused_heredocs(shell->ast, node->right);
// 			if (setup_redirections(node->right, shell) == -1)
// 				exit(1);
// 			execute_command(node->right, shell);
// 			cleanup_and_exit(shell, shell->last_exit_code);
// 		}
// 		close(pipe_fd[0]);
// 	}
// 	if (node->right->type != AST_PIPE)
// 	{
// 		waitpid(l_pid, &status, 0);
// 		if (WIFEXITED(status))
// 			shell->last_exit_code = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 		{
// 			shell->last_exit_code = 128 + WTERMSIG(status);
// 			if (WTERMSIG(status) == SIGINT)
// 				ft_putstr_fd("\n", STDERR_FILENO);
// 		}
// 		waitpid(r_pid, &status, 0);
// 		if (WIFEXITED(status))
// 			shell->last_exit_code = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 		{
// 			shell->last_exit_code = 128 + WTERMSIG(status);
// 			if (WTERMSIG(status) == SIGINT)
// 				ft_putstr_fd("\n", STDERR_FILENO);
// 		}
// 	}
// 	sig_interactive();
// }

pid_t	exec_left(t_ast *node, t_minishell *shell, int input_fd, int pipe_fd[2])
{
	pid_t	l_pid;

	l_pid = fork();
	if (l_pid == 0)
	{
		sig_reset(true);
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(pipe_fd[1]);
		if (input_fd != -1)
		{
			if (dup2(input_fd, STDIN_FILENO) == -1)
				shell->last_exit_code = 1;
			close(input_fd);
		}
		close_unused_heredocs(shell->ast, node->left);
		if (setup_redirections(node->left, shell) == -1)
			cleanup_and_exit(shell, 1);
		execute_command(node->left, shell);
		cleanup_and_exit(shell, shell->last_exit_code);
	}
	return (l_pid);
}

pid_t	exec_right(t_ast *node, t_minishell *shell, int pipe_fd[2])
{
	pid_t	r_pid;

	r_pid = fork();
	if (r_pid == 0)
	{
		sig_reset(true);
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			exit(1);
		close(pipe_fd[0]);
		close_unused_heredocs(shell->ast, node->right);
		if (setup_redirections(node->right, shell) == -1)
			cleanup_and_exit(shell, 1);
		execute_command(node->right, shell);
		cleanup_and_exit(shell, shell->last_exit_code);
	}
	close(pipe_fd[0]);
	return (r_pid);
}

// void	go_to_sleep(pid_t l_pid, pid_t r_pid, t_minishell *shell)
// {
// 	int	status;

// 	waitpid(l_pid, &status, 0);
// 	if (WIFEXITED(status))
// 		shell->last_exit_code = WEXITSTATUS(status);
// 	else if (WIFSIGNALED(status))
// 	{
// 		shell->last_exit_code = 128 + WTERMSIG(status);
// 		if (WTERMSIG(status) == SIGINT)
// 			ft_putstr_fd("\n", STDERR_FILENO);
// 	}
// 	waitpid(r_pid, &status, 0);
// 	if (WIFEXITED(status))
// 		shell->last_exit_code = WEXITSTATUS(status);
// 	else if (WIFSIGNALED(status))
// 	{
// 		shell->last_exit_code = 128 + WTERMSIG(status);
// 		if (WTERMSIG(status) == SIGINT)
// 			ft_putstr_fd("\n", STDERR_FILENO);
// 	}
// }

void	go_to_sleep(pid_t last_pid, t_minishell *shell)
{
	int		status;
	pid_t	pid;

	while ((pid = wait(&status)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				shell->last_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				shell->last_exit_code = 128 + WTERMSIG(status);
				// if (WTERMSIG(status) == SIGINT)
				// 	ft_putstr_fd("\n", STDERR_FILENO);
			}
		}
	}
}


void	execute_pipeline(t_ast *node, t_minishell *shell, int input_fd)
{
	int		pipe_fd[2];
	pid_t	l_pid;
	pid_t	r_pid;

	r_pid = 0;
	if (!node || node->type != AST_PIPE)
		return ;
	sig_reset(false);
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		shell->last_exit_code = 1;
		return ;
	}
	l_pid = exec_left(node, shell, input_fd, pipe_fd);
	close(pipe_fd[1]);
	if (input_fd != -1)
		close(input_fd);
	if (node->right->type == AST_PIPE)
		execute_pipeline(node->right, shell, pipe_fd[0]);
	else
		r_pid = exec_right(node, shell, pipe_fd);
	if (node->right->type != AST_PIPE)
		go_to_sleep(r_pid, shell);
	sig_interactive();
}
