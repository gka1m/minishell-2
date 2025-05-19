/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:22:17 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/19 14:03:17 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*extract_string(char *input, int *i)
{
	int		start;
	bool	in_single;
	bool	in_double;
	char	*str;

	in_single = false;
	in_double = false;
	start = *i;
	while (input[*i])
	{
		if (input[*i] == '\'' && !in_double)
			in_single = !in_single;
		else if (input[*i] == '"' && !in_single)
			in_double = !in_double;
		else if (!in_single && !in_double && (ft_isspace(input[*i])
				|| is_metachar(input[*i])))
			break ;
		(*i)++;
	}
	str = malloc(*i - start + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, input + start, *i - start + 1);
	str[*i - start] = '\0';
	return (str);
}

t_token	*tokenize_string(char *input, int *i)
{
	char	*str;
	t_token	*token;

	str = extract_string(input, i);
	if (!str || str[0] == '\0')
	{
		free(str);
		return (NULL);
	}
	token = create_token(str, T_STRING);
	if (!token)
		return (NULL);
	free(str);
	return (token);
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
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		if (is_invalid_redirection_sequence(input, i))
		{
			printf("minishell: syntax error near unexpected token `%.3s'\n",
				input + i);
			return (free_tokens(head), NULL);
		}
		// if (is_heredoc(input, i))
		// 	new_token = tokenize_hd(input, &i);
		// else if (is_append(input, i))
		// 	new_token = tokenize_append(input, &i);
		// else if (is_metachar(input[i]))
		// 	new_token = tokenize_metachar(input, &i);
		// else
		// 	new_token = tokenize_string(input, &i);
		new_token = classify_token(input, &i);
		if (!new_token)
			return (free_tokens(head), NULL);
		// if (!head)
		// 	head = new_token;
		// if (current)
		// 	current->next = new_token;
		// new_token->previous = current;
		// current = new_token;
		link_tokens(&head, &current, new_token);
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
