/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:59:01 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/05 13:06:17 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// function to create a new token and fill out the necessary information
t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	if (!new->value)
		return (free_tokens(new), NULL);
	new->type = type;
	new->adjacent = false;
	new->previous = NULL;
	new->next = NULL;
	return (new);
}

// void	free_tokens(t_token **head)
// {
// 	t_token	*temp;

// 	if (!head || !*head)
// 		return ;
// 	while (*head)
// 	{
// 		temp = *head;
// 		*head = (*head)->next;
// 		printf("FREE TOKEN %p [%s]\n", temp, temp->value);
// 		free(temp->value);
// 		free(temp);
// 	}
// 	*head = NULL;
// }

void	free_tokens(t_token *head)
{
	t_token	*tmp;
	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}



void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

bool	is_invalid_redirection_sequence(char *input, int i)
{
	if (!input[i])
		return (false);
	// Reject >>> or <<< (triple)
	if ((input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>')
		|| (input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<'))
		return (true);
	// Reject combinations like << <, >> >, > >>, < << etc.
	if ((input[i] == '<' || input[i] == '>') && (input[i + 1] == '<' || input[i
			+ 1] == '>') && (input[i + 2] == '<' || input[i + 2] == '>')
		&& !(input[i] == input[i + 1] && input[i + 1] == input[i + 2]))
		// allow << or >> only
		return (true);
	return (false);
}

void	toggle_quote_state(char c, int *in_dquote, int *in_squote)
{
	if (!(*in_squote) && c == '"')
		*in_dquote = !(*in_dquote);
	else if (!(*in_dquote) && c == '\'')
		*in_squote = !(*in_squote);
}
