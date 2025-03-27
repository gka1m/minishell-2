/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:06:24 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/27 14:43:13 by kagoh            ###   ########.fr       */
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

int	check_redirects(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		if (temp->type == T_REDIR_IN || temp->type == T_REDIR_OUT)
		{
			// Redirect must be followed by a filename
			if (!temp->next || temp->next->type != T_STRING)
			{
				printf("Redirect must be followed by a filename.\n");
				return (0);
			}
			// Redirect cannot be the last token
			if (!temp->next)
			{
				printf("Redirect cannot be the last token.\n");
				return (0);
			}
		}
		temp = temp->next;
	}
	return (1);
}

int	check_append(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		if (temp->type == T_APPEND)
		{
			// Append must be followed by a filename
			if (!temp->next || temp->next->type != T_STRING)
			{
				printf("Append must be followed by a filename.\n");
				return (0);
			}
			// Append cannot be the last token
			if (!temp->next)
			{
				printf("Append cannot be the last token.\n");
				return (0);
			}
		}
		temp = temp->next;
	}
	return (1);
}

int	check_heredoc(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		if (temp->type == T_HEREDOC)
		{
			// Heredoc must be followed by a delimiter
			if (!temp->next || temp->next->type != T_STRING)
			{
				printf("Heredoc must be followed by a delimiter.\n");
				return (0);
			}
		}
		temp = temp->next;
	}
	return (1);
}

int	check_eof(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp->next) // Traverse to the last token
		temp = temp->next;
	if (temp->type != T_EOF)
	{
		printf("Syntax error: Input must end with an EOF token.\n");
		return (0);
	}
	return (1);
}
