/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:48:32 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/08 16:08:14 by kagoh            ###   ########.fr       */
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
	node->hd_quoted = false;
	return (node);
}

void	free_ast(t_ast *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->args)
	{
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->file)
		free(node->file);
	free(node);
}

bool	is_redirection(t_token *token)
{
	if (!token)
		return (false);
	return (token->type == T_REDIR_IN || token->type == T_REDIR_OUT
		|| token->type == T_APPEND || token->type == T_HEREDOC);
}

bool	is_quoted(const char *str)
{
	if (!str || ft_strlen(str) < 2)
		return (false);
	return ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
		|| (str[0] == '"' && str[ft_strlen(str) - 1] == '"'));
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