/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:19:27 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/16 16:01:04 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_redirection(t_ast *node, t_minishell *shell)
{
	int fd;
	int flags;
	int original_fd;
	// int	backup;

	// backup = -1;
	if (!node || !node->file)
		return (-1);
	if (shell->stdio_backup[0] == -1) 
	{
		shell->stdio_backup[0] = dup(STDIN_FILENO);
		if (shell->stdio_backup[0] == -1) 
			return -1;
	}
	if (shell->stdio_backup[1] == -1) 
	{
		shell->stdio_backup[1] = dup(STDOUT_FILENO);
		if (shell->stdio_backup[1] == -1) 
		{
			close(shell->stdio_backup[0]);
			return -1;
		}
	}
	// Backup std fds
	// if (node->type == AST_REDIR_IN && shell->stdio_backup[0] == -1)
	// 	shell->stdio_backup[0] = dup(STDIN_FILENO);
	// if ((node->type == AST_REDIR_OUT || node->type == AST_APPEND)
	// 	&& shell->stdio_backup[1] == -1)
	// 	shell->stdio_backup[1] = dup(STDOUT_FILENO);

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
	
	// if ((original_fd == STDOUT_FILENO && shell->stdio_backup[1] == -1) ||
    //     (original_fd == STDIN_FILENO && shell->stdio_backup[0] == -1))
    // {
    //     backup = dup(original_fd);
    //     if (backup == -1)
    //     {
    //         perror("minishell: dup backup failed");
    //         return (-1);
    //     }
    //     // shell->stdio_backup[original_fd == STDOUT_FILENO ? 1 : 0] = backup;
	// 	if (original_fd == STDOUT_FILENO)
    //         shell->stdio_backup[1] = backup;
    //  	else
    //         shell->stdio_backup[0] = backup;
    // }

	fd = open(node->file, flags, 0644);
	if (fd == -1)
	{
		perror("minishell: redirection failed");
		// if (backup != -1)
		// {
		// 	close(backup);
		// 	shell->stdio_backup[original_fd] = -1;
		// }
		return (-1);
	}
	if (dup2(fd, original_fd) == -1)
	{
		perror("minishell: dup2 failed");
		// close(fd);
		// if (backup != -1)
		// {
		// 	close(backup);
		// 	shell->stdio_backup[original_fd] = -1;
		// }
		return (-1);
	}
	close(fd);
	return (0);
}

void	restore_standard_fds(t_minishell *shell)
{
	// if (shell->stdio_backup[0] != -1)
	// {
	// 	dup2(shell->stdio_backup[0], STDIN_FILENO);
	// 	close(shell->stdio_backup[0]);
	// 	shell->stdio_backup[0] = -1;
	// }
	// if (shell->stdio_backup[1] != -1)
	// {
	// 	dup2(shell->stdio_backup[1], STDOUT_FILENO);
	// 	close(shell->stdio_backup[1]);
	// 	shell->stdio_backup[1] = -1;
	// }
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

	// Traverse left to get all redirections (left-associative)
	// if (setup_redirections(node->right, shell) == -1)
	// 	return (-1);
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
			close(node->heredoc_fd);
			return (-1);
		}
		close(node->heredoc_fd);
		node->heredoc_fd = -1; // prevent double dup
	}

	// Continue to right in case of multiple chained redirections
	// if (setup_redirections(node->left, shell) == -1)
	// 	return (-1);
	if (setup_redirections(node->right, shell) == -1)
		return (-1);
	// restore_standard_fds(shell);
	return (0);
}

