/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:22:17 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 18:10:15 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*extract_token(char *input, int *i, t_token **head, t_token **current)
{
	t_token	*new_token;

	new_token = classify_token(input, i);
	if (!new_token)
	{
		free_tokens(*head);
		return (NULL);
	}
	link_tokens(head, current, new_token);
	return (new_token);
}

int	token_loop(char *input, int *i, t_token **head, t_token **current)
{
	if (ft_isspace(input[*i]))
	{
		(*i)++;
		return (1);
	}
	if (is_invalid_redirection_sequence(input, *i))
	{
		printf("minishell: syntax error near unexpected token `%.3s'\n", input
			+ *i);
		free_tokens(*head);
		return (0);
	}
	if (!extract_token(input, i, head, current))
		return (0);
	return (1);
}

t_token	*tokenize(char *input)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		i;
	int		len;

	head = NULL;
	current = NULL;
	new_token = NULL;
	i = 0;
	len = ft_strlen(input);
	while (i < len)
	{
		// if (ft_isspace(input[i]))
		// {
		// 	i++;
		// 	continue ;
		// }
		// if (is_invalid_redirection_sequence(input, i))
		// {
		// 	printf("minishell: syntax error near unexpected token `%.3s'\n",
		// 		input + i);
		// 	return (free_tokens(head), NULL);
		// }
		// if (!extract_token(input, &i, &head, &current))
		// 	return (NULL);
		if (!token_loop(input, &i, &head, &current))
			return (NULL);
	}
	return (head);
}

t_token	*classify_token(char *input, int *i)
{
	if (is_heredoc(input, *i))
		return (tokenize_hd(input, i));
	else if (is_append(input, *i))
		return (tokenize_append(input, i));
	else if (is_metachar(input[*i]))
		return (tokenize_metachar(input, i));
	else
		return (tokenize_string(input, i));
}

void	link_tokens(t_token **head, t_token **current, t_token *new_token)
{
	if (!*head)
		*head = new_token;
	if (*current)
		(*current)->next = new_token;
	new_token->previous = *current;
	*current = new_token;
}
