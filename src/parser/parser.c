/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:47:58 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/29 10:28:43 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast	*parse_pipeline(t_token **tokens, t_minishell *shell)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;
	t_token	*temp;

	temp = *tokens;
	left = parse_command(&temp, shell);
	if (!left)
		return (NULL);
	while (temp && temp->type == T_PIPE)
	{
		temp = temp->next;
		right = parse_pipeline(&temp, shell);
		if (!right)
			return (NULL);
		pipe_node = create_ast_node(AST_PIPE, shell);
		pipe_node->left = left;
		pipe_node->right = right;
		left = pipe_node;
	}
	return (left);
}

char	**append_arg(char **args, char *new_arg)
{
	size_t	len = 0;
	char	**new_args;

	while (args && args[len])
		len++;
	new_args = malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return (NULL);
	for (size_t i = 0; i < len; i++)
		new_args[i] = ft_strdup(args[i]);
	new_args[len] = ft_strdup(new_arg);
	new_args[len + 1] = NULL;
	if (args)
		free_split(args); // Free old array but not individual strings (we reused)
	return (new_args);
}

t_ast	*parse_command(t_token **tokens, t_minishell *shell)
{
	t_ast	*cmd_node = create_ast_node(AST_CMD, shell);
	t_ast	*result = cmd_node;
	t_ast	*redir_node;
	t_token	*temp = *tokens;

	if (!cmd_node)
		return (NULL);
	cmd_node->args = NULL;
	while (temp && temp->type != T_PIPE)
	{
		if (temp->type == T_STRING)
		{
			cmd_node->args = append_arg(cmd_node->args, temp->value);
			temp = temp->next;
		}
		else if (is_redirection(temp) || temp->type == T_HEREDOC)
		{
			if (temp->type == T_HEREDOC)
				redir_node = parse_heredoc(&temp, result, shell);
			else
				redir_node = parse_redirection(&temp, result, shell);
			if (!redir_node)
				return (free_ast(cmd_node), NULL);
			result = redir_node;
		}
		else
			temp = temp->next;
	}
	*tokens = temp;
	return (result);

}

// t_ast	*parse_command(t_token **tokens, t_minishell *shell)
// {
// 	t_ast	*cmd_node;
// 	t_ast	*result;
// 	t_ast	*redir_node;
// 	t_token	*temp;

// 	result = NULL;
// 	temp = *tokens;
// 	while (temp && temp->type != T_PIPE)
// 	{
		
// 		if (temp && temp->type == T_STRING)
// 		{
// 			cmd_node = create_ast_node(AST_CMD, shell);
// 			cmd_node->args = parse_arguments(&temp);
// 			result = cmd_node;
// 		}
// 		else if ((is_redirection(temp) || temp->type == T_HEREDOC))
// 		{
// 			if (temp->type == T_HEREDOC)
// 				redir_node = parse_heredoc(&temp, result, shell);
// 			else
// 				redir_node = parse_redirection(&temp, result, shell);
// 			if (!redir_node)
// 				return (free_ast(cmd_node), NULL);
// 			result = redir_node;
// 		}
// 	}
// 	*tokens = temp;
// 	return (result);
// }

char	**parse_arguments(t_token **tokens)
{
	char	**args;
	size_t	count;
	size_t	i;
	t_token	*temp;

	temp = *tokens;
	count = count_arguments(temp);
	if (count == 0)
		return (NULL);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count && temp && temp->type == T_STRING)
	{
		args[i] = ft_strdup(temp->value);
		if (!args[i])
			return (free_split(args), NULL);
		temp = temp->next;
		i++;
	}
	args[count] = NULL;
	*tokens = temp;
	return (args);
}

t_ast	*parse_redirection(t_token **tokens, t_ast *left, t_minishell *shell)
{
	t_ast		*redir_node;
	t_ast_type	type;
	t_token		*temp;

	temp = *tokens;
	if (temp->type == T_REDIR_IN)
		type = AST_REDIR_IN;
	else if (temp->type == T_REDIR_OUT)
		type = AST_REDIR_OUT;
	else if (temp->type == T_APPEND)
		type = AST_APPEND;
	else
		return (NULL);
	temp = temp->next;
	if (!temp || temp->type != T_STRING)
		return (NULL);
	redir_node = create_ast_node(type, shell);
	if (!redir_node)
		return (NULL);
	redir_node->file = ft_strdup(temp->value);
	if (!redir_node->file)
		return (free_ast(redir_node), free_ast(left), NULL);
	redir_node->left = left;
	*tokens = temp->next;
	return (redir_node);
}

bool	is_quoted_string(const char *str)
{
	size_t len = ft_strlen(str);
	return (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') || 
	                     (str[0] == '"' && str[len - 1] == '"')));
}

char	*remove_delim_q(const char *str)
{
	size_t len = ft_strlen(str);
	if (is_quoted_string(str))
		return (ft_substr(str, 1, len - 2));
	else
		return (ft_strdup(str));
}

t_ast	*parse_heredoc(t_token **tokens, t_ast *left, t_minishell *shell)
{
	t_token	*temp;
	t_ast	*heredoc_node;

	temp = *tokens;
	if (!temp || temp->type != T_HEREDOC)
		return (NULL);
	temp = temp->next;
	if (!temp || temp->type != T_STRING)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (free_ast(left), NULL);
	}
	heredoc_node = create_ast_node(AST_HEREDOC, shell);
	if (!heredoc_node)
		return (free_ast(left), NULL);
	heredoc_node->hd_quoted = is_quoted_string(temp->value);
	heredoc_node->file = remove_delim_q(temp->value);
	if (!heredoc_node->file)
		return (free_ast(heredoc_node), free_ast(left), NULL);
	// heredoc_node->hd_quoted = (temp->value[0] == '\'' || temp->value[0] == '"');
	heredoc_node->left = left;
	*tokens = temp->next;
	return (heredoc_node);
}
