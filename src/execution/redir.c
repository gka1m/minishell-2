/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:19:27 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/29 10:07:32 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_redirection(t_ast *node, t_minishell *shell)
{
	int	fd;
	int	flags;
	int	original_fd;

	if (!node || !node->file)
		return (-1);
	// if (shell->stdio_backup[0] == -1)
	// {
	// 	shell->stdio_backup[0] = dup(STDIN_FILENO);
	// 	if (shell->stdio_backup[0] == -1)
	// 		return (-1);
	// }
	// if (shell->stdio_backup[1] == -1)
	// {
	// 	shell->stdio_backup[1] = dup(STDOUT_FILENO);
	// 	if (shell->stdio_backup[1] == -1)
	// 	{
	// 		close(shell->stdio_backup[0]);
	// 		return (-1);
	// 	}
	// }
	backup_fds(shell);
	// if (node->type == AST_REDIR_IN)
	// {
	// 	flags = O_RDONLY;
	// 	original_fd = STDIN_FILENO;
	// }
	// else if (node->type == AST_REDIR_OUT)
	// {
	// 	flags = O_WRONLY | O_CREAT | O_TRUNC;
	// 	original_fd = STDOUT_FILENO;
	// }
	// else if (node->type == AST_APPEND)
	// {
	// 	flags = O_WRONLY | O_CREAT | O_APPEND;
	// 	original_fd = STDOUT_FILENO;
	// }
	// else
	// 	return (-1);
	if (get_redirection_info(node, &flags, &original_fd) == -1)
		return (-1);
	fd = open(node->file, flags, 0644);
	if (fd == -1)
	{
		perror("minishell: redirection failed");
		// free_tokens(shell->tokens);
		// free_ast(shell->ast);
		restore_standard_fds(shell);
		// free_minishell(shell);
		return (-1);
	}
	if (dup2(fd, original_fd) == -1)
		return (perror("minishell: dup2 failed"), -1);
	return (close(fd), 0);
}

int	get_redirection_info(t_ast *node, int *flags, int *original_fd)
{
	if (!node || !flags || !original_fd)
		return (-1);
	if (node->type == AST_REDIR_IN)
	{
		*flags = O_RDONLY;
		*original_fd = STDIN_FILENO;
	}
	else if (node->type == AST_REDIR_OUT)
	{
		*flags = O_WRONLY | O_CREAT | O_TRUNC;
		*original_fd = STDOUT_FILENO;
	}
	else if (node->type == AST_APPEND)
	{
		*flags = O_WRONLY | O_CREAT | O_APPEND;
		*original_fd = STDOUT_FILENO;
	}
	else
		return (-1);
	return (0);
}

void	restore_standard_fds(t_minishell *shell)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (shell->stdio_backup[i] != -1)
		{
			dup2(shell->stdio_backup[i], i);
			close(shell->stdio_backup[i]);
			shell->stdio_backup[i] = -1;
		}
		i++;
	}
}

void	backup_fds(t_minishell *shell)
{
	if (shell->stdio_backup[0] == -1)
	{
		shell->stdio_backup[0] = dup(STDIN_FILENO);
		if (shell->stdio_backup[0] == -1)
			return ;
	}
	if (shell->stdio_backup[1] == -1)
	{
		shell->stdio_backup[1] = dup(STDOUT_FILENO);
		if (shell->stdio_backup[1] == -1)
		{
			close(shell->stdio_backup[0]);
			return ;
		}
	}
}

int	setup_redirections(t_ast *node, t_minishell *shell)
{
	if (!node)
		return (0);
	if (setup_redirections(node->left, shell) == -1)
		return (-1);
	if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT
		|| node->type == AST_APPEND)
	{
		if (execute_redirection(node, shell) == -1)
			return (-1);
	}
	if (node->heredoc_fd >= 0)
	{
		if (shell->stdio_backup[0] == -1)
			shell->stdio_backup[0] = dup(STDIN_FILENO);
		if (dup2(node->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 heredoc");
			return (close(node->heredoc_fd), -1);
		}
		close(node->heredoc_fd);
		node->heredoc_fd = -1;
	}
	if (setup_redirections(node->right, shell) == -1)
		return (-1);
	return (0);
}
