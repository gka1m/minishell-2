/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:35:26 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/22 14:42:36 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_command(t_ast *node, t_minishell *shell)
{
	// pid_t	pid;
	// int		status;
	while (node && node->type != AST_CMD)
		node = node->left;
	if (!node || !node->args || !node->args[0])
		return ;
	sig_reset(false);
	if (is_builtin(node->args[0]))
	{
		if (setup_redirections(node, shell) == -1)
			exit(1);
		shell->last_exit_code = execute_builtin(shell, node->args, 1);
	}
	else
		not_bi(node, shell);
	// {
	// 	pid = fork();
	// 	if (pid == 0)
	// 	{
	// 		sig_reset(true);
	// 		if (setup_redirections(node, shell) == -1)
	// 			exit(1);
	// 		status = execute_external(node, shell);
	// 		cleanup_and_exit(shell, status);
	// 	}
	// 	else if (pid > 0)
	// 	{
	// 		handle_parent_process(pid, shell);
	// 		restore_standard_fds(shell);
	// 	}
	// 	else
	// 	{
	// 		perror("minishell: fork");
	// 		shell->last_exit_code = 1;
	// 	}
	// }
	sig_interactive();
}

void	not_bi(t_ast *node, t_minishell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		sig_reset(true);
		if (setup_redirections(node, shell) == -1)
			exit(1);
		status = execute_external(node, shell);
		cleanup_and_exit(shell, status);
	}
	else if (pid > 0)
	{
		handle_parent_process(pid, shell);
		restore_standard_fds(shell);
	}
	else
	{
		perror("minishell: fork");
		shell->last_exit_code = 1;
	}
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "echo") == 0 || strcmp(cmd,
			"env") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "pwd") == 0
		|| strcmp(cmd, "export") == 0 || strcmp(cmd, "unset") == 0);
}

void	handle_parent_process(pid_t pid, t_minishell *shell)
{
	int		status;
	pid_t	waited_pid;

	waited_pid = waitpid(pid, &status, 0);
	if (waited_pid == -1)
	{
		perror("minishell: waitpid");
		shell->last_exit_code = 1;
		return ;
	}
	if (WIFEXITED(status))
		shell->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->last_exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit: Core dumped\n", 18);
	}
}
