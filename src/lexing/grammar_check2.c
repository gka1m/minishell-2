/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_check2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:14:23 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/19 13:16:03 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_heredoc(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		if (temp->type == T_HEREDOC)
		{
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
		if (temp->type == T_REDIR_IN || temp->type == T_REDIR_OUT
			|| temp->type == T_APPEND || temp->type == T_HEREDOC)
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
