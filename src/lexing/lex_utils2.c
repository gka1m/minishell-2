/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:17:17 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/10 15:17:37 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

bool	is_redirection(t_token *token)
{
	if (!token)
		return (false);
	return (token->type == T_REDIR_IN || token->type == T_REDIR_OUT
		|| token->type == T_APPEND || token->type == T_HEREDOC);
}