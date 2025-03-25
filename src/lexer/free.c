/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:32:28 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/25 14:01:06 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_tokens(t_token **head)
{
	t_token	*temp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		temp = *head;
		*head = (*head)->next;
		free(temp->value);
		free(temp);
	}
	*head = NULL;
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

int	check_grammar(t_token *head)
{
	if (!check_quotes(head))
		return (0);
	if (!check_redirects(head))
		return (0);
	if (!check_append(head))
		return (0);
	if (!check_heredoc(head))
		return (0);
	if (!check_pipe(head))
		return (0);
	if (!check_eof(head))
		return (0);
	return (1);
}

int	check_quotes(t_token *tokens)
{
	t_token		*tmp;
	int			in_dquote;
	int			in_squote;
	char		*value;

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

void	toggle_quote_state(char c, int *in_dquote, int *in_squote)
{
	if (!(*in_squote) && c == '"')
		*in_dquote = !(*in_dquote);
	else if (!(*in_dquote) && c == '\'')
		*in_squote = !(*in_squote);
}
