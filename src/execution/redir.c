/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:19:27 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/04 11:23:47 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_redirection(t_ast *node, t_minishell *shell)
{
	int fd;
	int flags;
	int original_fd = -1;

	if (!node || !node->file)
		return (-1);
	if (node->type == AST_REDIR_IN) // <
	{
		flags = O_RDONLY;
		fd = open(node->file, flags);
		original_fd = STDIN_FILENO;
	}
	else if (node->type == AST_REDIR_OUT) // >
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(node->file, flags, 0644);
		original_fd = STDOUT_FILENO;
	}
	else if (node->type == AST_APPEND) // >>
	{
		flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(node->file, flags, 0644);
		original_fd = STDOUT_FILENO;
	}
	else if (node->type == AST_HEREDOC) // <<
	{
		fd = execute_heredoc(node->file, shell);
		original_fd = STDIN_FILENO;
	}
	else
		return (-1);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (-1);
	}
	if (dup2(fd, original_fd) == -1)
		return (close(fd), -1);
	return (close(fd), 0);
}

int setup_redirections(t_ast *node, t_minishell *shell)
{
    t_ast *current;

    if (!node)
        return (-1);

    // Handle current node's redirection
    if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT || 
        node->type == AST_APPEND || node->type == AST_HEREDOC)
    {
        if (execute_redirection(node, shell) == -1)
            return (-1);
    }

    // Process left subtree (redirections come before commands)
    if (node->left && setup_redirections(node->left, shell) == -1)
        return (-1);

    // Process right subtree (for pipes or chained redirections)
    if (node->right && setup_redirections(node->right, shell) == -1)
        return (-1);

    return (0);
}