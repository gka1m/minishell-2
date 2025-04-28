/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:47:58 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/28 17:21:40 by kagoh            ###   ########.fr       */
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
	t_ast	*redir_node;

	cmd_node = NULL;
	result = NULL;
	// Only create CMD node if we have arguments or no redirections
	if (*tokens && (*tokens)->type == T_STRING)
	{
		cmd_node = create_ast_node(AST_CMD, shell);
		// if (!cmd_node)
		// 	return (NULL);
		cmd_node->args = parse_arguments(tokens);
		result = cmd_node;
	}
	// Handle redirections
	while (*tokens && (is_redirection(*tokens) || (*tokens)->type == T_HEREDOC))
	{
		if ((*tokens)->type == T_HEREDOC)
			redir_node = parse_heredoc(tokens, result, shell);
		else
			redir_node = parse_redirection(tokens, result, shell);
		if (!redir_node)
			return (free_ast(cmd_node), NULL);
		result = redir_node;
	}
	// If we only had redirections, return the last redirection node
	// if (!cmd_node)
	// 	return (result);
	// If we had both command and redirections, return the structure
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
	t_token		*redir_token;
	t_token		*file_token;
	t_ast		*redir_node;
	t_ast_type	type;

	redir_token = *tokens;
	if (redir_token->type == T_REDIR_IN)
		type = AST_REDIR_IN;
	else if (redir_token->type == T_REDIR_OUT)
		type = AST_REDIR_OUT;
	else if (redir_token->type == T_APPEND)
		type = AST_APPEND;
	else
		return (free_ast(left), NULL);
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != T_STRING)
		return (free_ast(left), NULL);
	file_token = *tokens;
	*tokens = (*tokens)->next;
	redir_node = create_ast_node(type, shell);
	if (!redir_node)
		return (free_ast(left), NULL);
	redir_node->file = ft_strdup(file_token->value);
	if (!redir_node->file)
		return (free_ast(redir_node), free_ast(left), NULL);
	redir_node->left = left;
	return (redir_node);
}

t_ast	*parse_heredoc(t_token **tokens, t_ast *left, t_minishell *shell)
{
	t_token	*delim_token;
	t_ast	*heredoc_node;

	if (!tokens || !*tokens || (*tokens)->type != T_HEREDOC)
		return (NULL);
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != T_STRING)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (free_ast(left), NULL);
	}
	delim_token = *tokens;
	*tokens = (*tokens)->next;
	heredoc_node = create_ast_node(AST_HEREDOC, shell);
	if (!heredoc_node)
		return (free_ast(left), NULL);
	heredoc_node->file = ft_strdup(delim_token->value);
	if (!heredoc_node->file)
		return (free_ast(heredoc_node), free_ast(left), NULL);
	heredoc_node->hd_quoted = (delim_token->value[0] == '\''
			|| delim_token->value[0] == '"');
	heredoc_node->left = left;
	return (heredoc_node);
}
