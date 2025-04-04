/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:59:01 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/04 14:01:34 by kagoh            ###   ########.fr       */
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
		return (free(new), NULL);
	new->type = type;
	new->previous = NULL;
	new->next = NULL;
	return (new);
}

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

bool	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

bool	is_heredoc(char *input, int i)
{
	return (input[i] == '<' && input[i + 1] == '<');
}

bool	is_append(char *input, int i)
{
	return (input[i] == '>' && input[i + 1] == '>');
}

int ft_isspace(int c)
{
    return (c == ' '  || c == '\t' || c == '\n' \
            || c == '\v' || c == '\f' || c == '\r');
}