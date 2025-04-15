/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:19:27 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/15 16:14:00 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int saved_stdin = -1;
static int saved_stdout = -1;

int execute_redirection(t_ast *node, t_minishell *shell)
{
    int fd;
    int flags;
    int original_fd = -1;

    (void)shell;
    if (!node || !node->file)
        return (-1);

    // Save original descriptors on first redirection
    if (node->type == AST_REDIR_IN && saved_stdin == -1)
        saved_stdin = dup(STDIN_FILENO);
    if ((node->type == AST_REDIR_OUT || node->type == AST_APPEND) && saved_stdout == -1)
        saved_stdout = dup(STDOUT_FILENO);

    if (node->type == AST_REDIR_IN) {
        flags = O_RDONLY;
        fd = open(node->file, flags);
        original_fd = STDIN_FILENO;
    }
    else if (node->type == AST_REDIR_OUT) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
        fd = open(node->file, flags, 0644);
        original_fd = STDOUT_FILENO;
    }
    else if (node->type == AST_APPEND) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
        fd = open(node->file, flags, 0644);
        original_fd = STDOUT_FILENO;
    }
    else {
        return (-1);
    }

    if (fd == -1) {
        perror("minishell: redirection failed");
        return (-1);
    }

    if (dup2(fd, original_fd) == -1) {
        perror("minishell: dup2 failed");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

void restore_standard_fds()
{
    if (saved_stdin != -1) {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
        saved_stdin = -1;
    }
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        saved_stdout = -1;
    }
}

int	setup_redirections(t_ast *node, t_minishell *shell)
{
	if (!node)
		return (0);
	// Process left subtree first (redirections)
	if (setup_redirections(node->left, shell) == -1)
		return (-1);
	// Handle current node's redirection
	if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT
		|| node->type == AST_APPEND)
	{
		if (execute_redirection(node, shell) == -1)
			return (-1);
	}
	// Process right subtree (commands)
	if (setup_redirections(node->right, shell) == -1)
		return (-1);
	return (0);
}
