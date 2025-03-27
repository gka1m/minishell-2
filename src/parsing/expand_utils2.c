/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:16:13 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/27 13:07:59 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_strjoin_char(char *s1, char c)
{
	char	*result;
	int		len;
	int		i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	result = malloc(len + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	result[i++] = c;
	result[i] = '\0';
	free(s1);
	return (result);
}

// Handles special characters: single quotes, double quotes, and variables
// for expand_input fn
int	handle_specials(char *input, int *i, t_minishell *shell, char **result)
{
	if (input[*i] == '\'')
		return (handle_squote(input, i, result));
	else if (input[*i] == '\"')
		return (handle_dquote(input, i, shell, result));
	else if (input[*i] == '$')
	{
		(*i)++;
		return (expand_var(input, i, shell, result));
	}
	return (1);
}
