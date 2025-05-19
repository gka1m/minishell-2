/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:59:23 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/19 13:15:38 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// int	check_pipe(t_token *head)
// {
// 	t_token	*temp;

// 	temp = head;
// 	while (temp)
// 	{
// 		if (temp->type == T_PIPE)
// 		{
// 			if (!temp->previous || !temp->next)
// 			{
// 				printf("Pipe cannot be first or last token\n");
// 				return (0);
// 			}
// 			if (temp->previous->type != T_STRING
// 				|| temp->next->type != T_STRING)
// 			{
// 				printf("Pipe must have commands on both sides.\n");
// 				return (0);
// 			}
// 		}
// 		temp = temp->next;
// 	}
// 	return (1);
// }
int	is_valid_pipe_side(t_token *token, int direction)
{
	while (token)
	{
		if (token->type == T_STRING)
			return (1);
		if (token->type == T_PIPE)
			break ;
		if (direction == -1)
			token = token->previous;
		else
			token = token->next;
	}
	return (0);
}

int	check_pipe(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		if (temp->type == T_PIPE)
		{
			if (!temp->previous || !temp->next || temp->next->type == T_PIPE)
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				return (0);
			}
			if (!is_valid_pipe_side(temp->previous, -1)
				|| !is_valid_pipe_side(temp->next, 1))
			{
				printf("minishell: invalid null command\n");
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
