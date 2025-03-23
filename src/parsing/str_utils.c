/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 15:58:21 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/23 16:42:19 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Helper function to check if a character is a special character
bool	is_special_char(char c)
{
	return (c == '\'' || c == '\"' || c == '$');
}

// Helper function to expand variables (simplified for this example)
char	*expand_variable(const char *input, t_minishell *shell)
{
	char	var_name[256];
	int		i;

	i = 0;
	if (input[i] == '?')
	{
		var_name[i++] = '?';
		var_name[i] = '\0';
		return (ft_itoa(shell->last_exit_code));
	}
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
	{
		var_name[i] = input[i];
		i++;
	}
	var_name[i] = '\0';
	return (getenv(var_name));
}

void	single_quotes(const char *input, int *i, bool *in_single_quotes)
{
	*in_single_quotes = !(*in_single_quotes);
	(*i)++;
}

void	double_quotes(const char *input, int *i, bool *in_double_quotes)
{
	*in_double_quotes = !(*in_double_quotes);
	(*i)++;
}
