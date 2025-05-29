/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:48:32 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/29 10:11:41 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast	*create_ast_node(t_ast_type type, t_minishell *shell)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->file = NULL;
	node->left = NULL;
	node->right = NULL;
	node->shell = shell;
	node->heredoc_fd = -1;
	// node->hd_quoted = false;hd_quoted
	return (node);
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->type == AST_CMD && node->args)
	{
		free_split(node->args);
	}
	if (node->file)
		free(node->file);
	if (node->heredoc_fd != -1)
	{
		close(node->heredoc_fd);
		node->heredoc_fd = -1;
	}
	free(node);
}

size_t	count_arguments(t_token *tokens)
{
	size_t	count;

	count = 0;
	while (tokens && tokens->type == T_STRING)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

t_ast_type	classify_redir(t_token_type token_type)
{
	if (token_type == T_REDIR_IN)
		return (AST_REDIR_IN);
	if (token_type == T_REDIR_OUT)
		return (AST_REDIR_OUT);
	if (token_type == T_APPEND)
		return (AST_APPEND);
	return (AST_CMD);
}
