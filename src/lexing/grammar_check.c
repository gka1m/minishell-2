/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:59:23 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/10 15:17:43 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_pipe(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		if (temp->type == T_PIPE)
		{
			if (!temp->previous || !temp->next)
			{
				printf("Pipe cannot be first or last token\n");
				return (0);
			}
			if (temp->previous->type != T_STRING
				|| temp->next->type != T_STRING)
			{
				printf("Pipe must have commands on both sides.\n");
				return (0);
			}
		}
		temp = temp->next;
	}
	return (1);
}

// int	check_redirects(t_token *head)
// {
// 	t_token	*temp;

// 	temp = head;
// 	while (temp)
// 	{
// 		if (temp->type == T_REDIR_IN || temp->type == T_REDIR_OUT)
// 		{
// 			// Redirect must be followed by a filename
// 			if (!temp->next || temp->next->type != T_STRING)
// 			{
// 				printf("Redirect must be followed by a filename.\n");
// 				return (0);
// 			}
// 			// Redirect cannot be the last token
// 			if (!temp->next)
// 			{
// 				printf("Redirect cannot be the last token.\n");
// 				return (0);
// 			}
// 		}
// 		temp = temp->next;
// 	}
// 	return (1);
// }

int	check_redirects(t_token *token)
{
	if (!token->next)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (0);
	}
	if (token->next->type == T_PIPE)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	if (token->next->type == T_REDIR_IN || token->next->type == T_REDIR_OUT
		|| token->next->type == T_APPEND)
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			token->next->value);
		return (0);
	}
	if (token->next->type == T_STRING && token->next->value[0] == '\0')
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (0);
	}
	return (1);
}

// int	check_append(t_token *head)
// {
// 	t_token	*temp;

// 	temp = head;
// 	while (temp)
// 	{
// 		if (temp->type == T_APPEND)
// 		{
// 			// Append must be followed by a filename
// 			if (!temp->next || temp->next->type != T_STRING)
// 			{
// 				printf("Append must be followed by a filename.\n");
// 				return (0);
// 			}
// 			// Append cannot be the last token
// 			if (!temp->next)
// 			{
// 				printf("Append cannot be the last token.\n");
// 				return (0);
// 			}
// 		}
// 		temp = temp->next;
// 	}
// 	return (1);
// }

int	check_heredoc(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		if (temp->type == T_HEREDOC)
		{
			// Heredoc must be followed by a delimiter
			if (!temp->next || temp->next->type != T_STRING
				|| !temp->next->value[0])
			{
				printf("Heredoc must be followed by a delimiter.\n");
				return (0);
			}
		}
		temp = temp->next;
	}
	return (1);
}

int	check_grammar(t_token *head)
{
	t_token	*temp;

	temp = head;
	if (!check_quotes(head))
		return (0);
	if (!check_pipe(head))
		return (0);
	while (temp)
	{
		if (temp->type == T_REDIR_IN || temp->type == T_REDIR_OUT || 
			temp->type == T_APPEND || temp->type == T_HEREDOC)
		{
			if (!check_redirects(temp))
				return (0);
		}
		temp = temp->next;
	}
	if (!check_heredoc(head))
		return (0);
	return (1);
}

int	check_quotes(t_token *tokens)
{
	t_token	*tmp;
	int		in_dquote;
	int		in_squote;
	char	*value;

	tmp = tokens;
	in_dquote = 0;
	in_squote = 0;
	while (tmp)
	{
		value = tmp->value;
		if (tmp->type == T_STRING)
		{
			while (*value)
			{
				toggle_quote_state(*value, &in_dquote, &in_squote);
				value++;
			}
		}
		tmp = tmp->next;
	}
	if (in_dquote || in_squote)
		return (printf("syntax error: unmatched quotes\n"), 0);
	return (1);
}
