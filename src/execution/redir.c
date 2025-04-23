/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:19:27 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/23 13:16:46 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_redirection(t_ast *node, t_minishell *shell)
{
	int fd;
	int flags;
	int original_fd;

	if (!node || !node->file)
		return (-1);

	// Backup std fds
	if (node->type == AST_REDIR_IN && shell->stdio_backup[0] == -1)
		shell->stdio_backup[0] = dup(STDIN_FILENO);
	if ((node->type == AST_REDIR_OUT || node->type == AST_APPEND)
		&& shell->stdio_backup[1] == -1)
		shell->stdio_backup[1] = dup(STDOUT_FILENO);

	if (node->type == AST_REDIR_IN)
	{
		flags = O_RDONLY;
		original_fd = STDIN_FILENO;
	}
	else if (node->type == AST_REDIR_OUT)
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		original_fd = STDOUT_FILENO;
	}
	else if (node->type == AST_APPEND)
	{
		flags = O_WRONLY | O_CREAT | O_APPEND;
		original_fd = STDOUT_FILENO;
	}
	else
		return (-1);

	fd = open(node->file, flags, 0644);
	if (fd == -1)
	{
		perror("minishell: redirection failed");
		return (-1);
	}
	if (dup2(fd, original_fd) == -1)
	{
		perror("minishell: dup2 failed");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}


void	restore_standard_fds(t_minishell *shell)
{
	if (shell->stdio_backup[0] != -1)
	{
		dup2(shell->stdio_backup[0], STDIN_FILENO);
		close(shell->stdio_backup[0]);
		shell->stdio_backup[0] = -1;
	}
	if (shell->stdio_backup[1] != -1)
	{
		dup2(shell->stdio_backup[1], STDOUT_FILENO);
		close(shell->stdio_backup[1]);
		shell->stdio_backup[1] = -1;
	}
}


int	setup_redirections(t_ast *node, t_minishell *shell)
{
	if (!node)
		return (0);

	// Traverse left to get all redirections (left-associative)
	if (setup_redirections(node->left, shell) == -1)
		return (-1);
	if (node->heredoc_fd != -1)
	{
		if (shell->stdio_backup[0] == -1)
			shell->stdio_backup[0] = dup(STDIN_FILENO);
	
		if (dup2(node->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 heredoc");
			close(node->heredoc_fd);
			return (-1);
		}
		close(node->heredoc_fd);
		node->heredoc_fd = -1; // prevent double dup
	}
	if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT
		|| node->type == AST_APPEND)
	{
		if (execute_redirection(node, shell) == -1)
			return (-1);
	}

	// Continue to right in case of multiple chained redirections
	if (setup_redirections(node->right, shell) == -1)
		return (-1);

	return (0);
}

