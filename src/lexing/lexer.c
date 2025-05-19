/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:01:42 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/19 13:23:44 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*tokenize_hd(char *input, int *i)
{
	t_token	*token;

	(void)input;
	token = create_token("<<", T_HEREDOC);
	*i += 2;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	return (token);
}

t_token	*tokenize_append(char *input, int *i)
{
	t_token	*token;

	(void)input;
	token = create_token(">>", T_APPEND);
	*i += 2;
	return (token);
}

t_token	*tokenize_metachar(char *input, int *i)
{
	t_token_type	type;
	char			op[2];
	t_token			*token;

	op[0] = input[*i];
	op[1] = '\0';
	if (op[0] == '<')
		type = T_REDIR_IN;
	else if (op[0] == '>')
		type = T_REDIR_OUT;
	else
		type = T_PIPE;
	token = create_token(op, type);
	(*i)++;
	return (token);
}

// t_token	*tokenize(char *input)
// {
// 	t_token	*head;
// 	t_token	*current;
// 	int		i;
// 	int		len;

// 	head = NULL;
// 	current = NULL;
// 	i = 0;
// 	len = ft_strlen(input);
// 	while (i < len)
// 	{
// 		if (ft_isspace(input[i]))
// 			i++;
// 		if (is_invalid_redirection_sequence(input, i))
// 		{
// 			printf("minishell: syntax error near unexpected token `%.3s'\n",
// 				input + i);
// 			return (NULL);
// 		}
// 		if (is_heredoc(input, i))
// 		{
// 			current = tokenize_hd(input, &i, current);
// 			if (!head)
// 				head = current;
// 			continue ;
// 		}
// 		if (is_append(input, i))
// 		{
// 			current = tokenize_append(input, &i, current);
// 			if (!head)
// 				head = current;
// 			continue ;
// 		}
// 		if (is_metachar(input[i]))
// 		{
// 			current = tokenize_metachar(input, &i, current);
// 			if (!head)
// 				head = current;
// 			continue ;
// 		}
// 		current = tokenize_string(input, &i, current);
// 		if (!head && current)
// 			head = current;
// 	}
// 	free_tokens(&current);
// 	return (head);
// }
