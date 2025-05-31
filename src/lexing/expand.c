/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:57:25 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 18:19:07 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_char(char c, int *in_squote, int *in_dquote, char **result)
{
	char	*temp;
	char	ch[2];

	if ((c == '\'' && !(*in_dquote)) || (c == '"' && !(*in_squote)))
	{
		toggle_quote_state(c, in_dquote, in_squote);
		return (1);
	}
	ch[0] = c;
	ch[1] = '\0';
	temp = ft_strjoin(*result, ch);
	free(*result);
	*result = temp;
	return (1);
}

// char	*remove_quotes(char *str)
// {
// 	char	*result;
// 	int		in_squote;
// 	int		in_dquote;
// 	size_t	i;

// 	result = ft_strdup("");
// 	in_squote = 0;
// 	in_dquote = 0;
// 	i = 0;
// 	if (!str)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		process_char(str[i], &in_squote, &in_dquote, &result);
// 		i++;
// 	}
// 	return (result);
// }

// char	*remove_outer_quotes(char *str)
// {
// 	size_t	len;
// 	char	*result;

// 	if (!str)
// 		return (NULL);
// 	len = ft_strlen(str);
// 	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"'
// 				&& str[len - 1] == '"')))
// 		result = ft_substr(str, 1, len - 2);
// 	else
// 		result = ft_strdup(str);
// 	if (!result)
// 		result = ft_strdup("");
// 	return (result);
// }

void	process_str(t_token *token, t_minishell *shell)
{
	char	*expanded;

	expanded = expand_variables_with_quotes(token->value, shell);
	free(token->value);
	token->value = expanded;
}

// Fixed expand_all_tokens
t_token	*expand_all_tokens(t_token *tokens, t_minishell *shell)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redirection(current))
		{
			current = handle_redirection_expansion(current, shell);
			if (current)
				current = current->next;
		}
		else if (current->type == T_STRING)
		{
			if (current->previous && current->previous->type == T_HEREDOC)
			{
				current = current->next;
				continue ;
			}
			process_str(current, shell);
			current = current->next;
		}
		else
			current = current->next;
	}
	return (concatenate_adjacent_strings(tokens));
}

// Simplified redirection handler
t_token	*handle_redirection_expansion(t_token *token, t_minishell *shell)
{
	t_token	*file_token;

	if (!token || !is_redirection(token))
		return (token);
	file_token = token->next;
	if (!file_token || file_token->type != T_STRING)
		return (token);
	expand_variables(file_token, shell);
	return (token);
}

t_token	*concatenate_adjacent_strings(t_token *tokens)
{
	t_token	*curr;
	t_token	*next;
	char	*temp;

	curr = tokens;
	while (curr && curr->next)
	{
		next = curr->next;
		if (curr->type == T_STRING && next->type == T_STRING && next->adjacent)
		{
			temp = ft_strjoin(curr->value, next->value);
			free(curr->value);
			curr->value = temp;
			curr->next = next->next;
			if (next->next)
				next->next->previous = curr;
			free(next->value);
			free(next);
		}
		else
		{
			curr = curr->next;
		}
	}
	return (tokens);
}
