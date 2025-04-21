/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:08:58 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/21 17:33:21 by kagoh            ###   ########.fr       */
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
		sig_interactive();
		return ;
	}
	// Left command (writes to pipe)
	pid = fork();
	if (pid == 0)
	{
		// Child process setup
		sig_reset(true);
		close(pipe_fd[0]); // Close unused read end
		// Setup redirections (including heredoc if present)
		// Connect stdout to pipe
		dup2(pipe_fd[1], STDOUT_FILENO);
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
		sig_interactive();
		return ;
	}
	// Right side preparation
	close(pipe_fd[1]);    // Close write end in parent
	left_fd = pipe_fd[0]; // Save read end for next command
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
			// Child process setup
			sig_reset(true);
			// Connect stdin to pipe
			dup2(left_fd, STDIN_FILENO);
			close(left_fd);
			// Setup other redirections
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
			sig_interactive();
			return ;
		}
	}
	// Cleanup in parent
	close(left_fd);
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

// void execute_pipeline(t_ast *node, t_minishell *shell)
// {
//     int pipe_fd[2];
//     pid_t pid;
//     t_pipechild pc;

//     if (!node || node->type != AST_PIPE)
//         return execute_command(node, shell);

//     // Setup pipe
//     if (pipe(pipe_fd) == -1) {
//         perror("minishell: pipe");
//         shell->last_exit_code = 1;
//         return;
//     }

//     /* Left Command (writes to pipe) */
//     pc = (t_pipechild){
//         .ast = node->left,
//         .minishell = shell,
//         .read_fd = STDIN_FILENO,
//         .write_fd = pipe_fd[1],
//         .std_fd = STDIN_FILENO
//     };
//     pid = fork();
//     if (pid == 0) {
//         close(pipe_fd[0]);
//         execute_pipe_segment(&pc);
//         exit(shell->last_exit_code);
//     } else if (pid < 0) {
//         perror("minishell: fork");
//         close(pipe_fd[0]);
// 		close(pipe_fd[1]);
//         shell->last_exit_code = 1;
//         return;
//     }

//     /* Right Command (reads from pipe) */
//     close(pipe_fd[1]); // Close write end in parent
//     pc = (t_pipechild){
//         .ast = node->right,
//         .minishell = shell,
//         .read_fd = pipe_fd[0],
//         .write_fd = STDOUT_FILENO,
//         .std_fd = pipe_fd[0]
//     };

//     if (node->right->type == AST_PIPE) {
//         execute_pipeline(node->right, shell);
//     } else {
//         pid = fork();
//         if (pid == 0) {
//             execute_pipe_segment(&pc);
//             exit(shell->last_exit_code);
//         } else if (pid < 0) {
//             perror("minishell: fork");
//             close(pipe_fd[0]);
//             shell->last_exit_code = 1;
//         }
//     }

//     // Cleanup and wait
//     close(pipe_fd[0]);
//     wait_for_children(shell);
// }

// void execute_pipe_segment(t_pipechild *pc)
// {
//     // Set up file descriptors
//     if (pc->read_fd != STDIN_FILENO) {
//         dup2(pc->read_fd, STDIN_FILENO);
//         close(pc->read_fd);
//     }
//     if (pc->write_fd != STDOUT_FILENO) {
//         dup2(pc->write_fd, STDOUT_FILENO);
//         close(pc->write_fd);
//     }

//     // Handle heredoc if exists (must come before other redirections)
//     if (pc->ast->heredoc_fd != -1) {
//         dup2(pc->ast->heredoc_fd, STDIN_FILENO);
//         close(pc->ast->heredoc_fd);
//     }

//     // Setup other redirections
//     if (setup_redirections(pc->ast, pc->minishell) == -1)
//         exit(1);

//     // Execute the command
//     execute_command(pc->ast, pc->minishell);
// }