/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:14:13 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/26 12:38:35 by kagoh            ###   ########.fr       */
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
	char	*combined;
	char	*last_content;
	char	*new_content;
	char	*final;

	// Extract content without surrounding quotes
	last_content = ft_substr(last->value, 1, ft_strlen(last->value) - 2);
	new_content = ft_substr(new->value, 1, ft_strlen(new->value) - 2);
	// Combine the contents
	combined = ft_strjoin(last_content, new_content);
	// Re-add the quotes from the first string
	final = ft_strjoin("\"", combined);
	final = ft_strjoin_free(final, "\"");
	// Update the token
	free(last->value);
	last->value = final;
	// Clean up
	free(last_content);
	free(new_content);
	free(combined);
	free(new->value);
	free(new);
}
