/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:14:13 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/27 13:13:54 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	should_concatenate(t_token *last, char *current_str)
{
	// Never concatenate if there's no previous token
	if (!last)
		return (0);
	// Only concatenate string tokens
	if (last->type != T_STRING)
		return (0);
	// Only concatenate if both are quoted strings (either single or double)
	if ((last->value[0] == '\'' || last->value[0] == '"')
			&& (current_str[0] == '\'' || current_str[0] == '"'))
		return (1);
	return (0);
}

void	concatenate_quoted_strings(t_token *last, t_token *new)
{
	char	*last_content;
	char	*new_content;
	char	*combined;

	last_content = last->value;
	new_content = new->value;
	// Case 1: Both strings use the same quote type
	if ((last->value[0] == '\'' && new->value[0] == '\'')
		|| (last->value[0] == '"' && new->value[0] == '"'))
		// Combine without stripping inner quotes
		combined = ft_strjoin(last_content, new_content + 1);
			// Skip opening quote of new
	// Case 2: Mixed quotes (e.g., "hello" + '$USER')
	else
		// Preserve both segments verbatim (no stripping quotes)
		combined = ft_strjoin(last_content, new_content);
	// Update the token
	free(last->value);
	last->value = combined;
	free(new->value);
	free(new);
}
