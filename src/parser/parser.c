/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:47:58 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/08 16:04:31 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast	*parse_pipeline(t_token **tokens, t_minishell *shell)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(tokens, shell);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == T_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens, shell);
		if (!right)
			return (NULL);
		pipe_node = create_ast_node(AST_PIPE, shell);
		pipe_node->left = left;
		pipe_node->right = right;
		left = pipe_node;
	}
	return (left);
}

t_ast	*parse_command(t_token **tokens, t_minishell *shell)
{
	t_ast	*cmd_node;
	t_ast	*result;

	cmd_node = create_ast_node(AST_CMD, shell);
	if (!cmd_node)
		return (NULL);
	cmd_node->args = parse_arguments(tokens);
	if (!cmd_node->args && *tokens && (*tokens)->type != T_PIPE
		&& !is_redirection(*tokens))
	{
		free_ast(cmd_node);
		return (NULL);
	}
	result = cmd_node;
	while (*tokens && is_redirection(*tokens))
	{
		result = parse_redirection(tokens, result, shell);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	**parse_arguments(t_token **tokens)
{
	char	**args;
	size_t	count;
	size_t	i;

	count = count_arguments(*tokens);
	if (count == 0)
		return (NULL);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count && *tokens && (*tokens)->type == T_STRING)
	{
		args[i] = ft_strdup((*tokens)->value);
		if (!args[i])
		{
			while (i > 0)
				free(args[--i]);
			free(args);
			return (NULL);
		}
		(*tokens) = (*tokens)->next;
		i++;
	}
	args[count] = NULL;
	return (args);
}

t_ast	*parse_redirection(t_token **tokens, t_ast *left, t_minishell *shell)
{
	t_token *redir_token;
	t_token *file_token;
	t_ast *redir_node;
	t_ast_type type;

	redir_token = *tokens;
	// Directly assign AST type based on token type
	if (redir_token->type == T_REDIR_IN)
		type = AST_REDIR_IN;
	else if (redir_token->type == T_REDIR_OUT)
		type = AST_REDIR_OUT;
	else if (redir_token->type == T_APPEND)
		type = AST_APPEND;
	else if (redir_token->type == T_HEREDOC)
		type = AST_HEREDOC;
	else
	{
		free_ast(left);
		return (NULL); // Not a redirection token
	}
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != T_STRING)
	{
		free_ast(left);
		return (NULL); // Missing filename
	}
	file_token = *tokens;
	*tokens = (*tokens)->next;
	redir_node = create_ast_node(type, shell);
	if (!redir_node)
	{
		free_ast(left);
		return (NULL);
	}
	redir_node->file = ft_strdup(file_token->value);
	if (!redir_node->file)
	{
		free_ast(redir_node);
		free_ast(left);
		return (NULL);
	}
	redir_node->left = left;
	// Handle heredoc quote status
	if (type == AST_HEREDOC)
		redir_node->hd_quoted = (file_token->value[0] == '\''
				|| file_token->value[0] == '"');
	return (redir_node);
}

