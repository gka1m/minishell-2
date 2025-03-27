/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:15:12 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/27 13:01:51 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	expand_squote_redir(char **input, int *i, char **result)
{
	(*i)++; // Skip opening quote
	while ((*input)[*i] && (*input)[*i] != '\'')
	{
		*result = ft_strjoin_char(*result, (*input)[*i]);
		(*i)++;
	}
	if ((*input)[*i] == '\'')
		(*i)++; // Skip closing quote
}

void	expand_dquote_redir(char **input, int *i, t_minishell *shell,
		char **result)
{
	(*i)++; // Skip opening quote
	while ((*input)[*i] && (*input)[*i] != '"')
	{
		if ((*input)[*i] == '$')
			expand_var(*input, i, shell, result);
		// Reuse your existing function
		else
		{
			*result = ft_strjoin_char(*result, (*input)[*i]);
			(*i)++;
		}
	}
	if ((*input)[*i] == '"')
		(*i)++; // Skip closing quote
}

char	*expand_redirection_target(char *input, t_minishell *shell)
{
	char *result = ft_strdup("");
	int i = 0;

	while (input[i])
	{
		if (input[i] == '\'')
			expand_squote_redir(&input, &i, &result);
		else if (input[i] == '"')
			expand_dquote_redir(&input, &i, shell, &result);
		else if (input[i] == '$')
			expand_var(input, &i, shell, &result);
				// Reuse your existing function
		else
		{
			result = ft_strjoin_char(result, input[i]);
			i++;
		}
	}
	return (result);
}