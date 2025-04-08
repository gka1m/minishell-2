/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:20:15 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/08 15:33:15 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_var_char(char c, int first_char)
{
	if (first_char)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

char	*extract_var_name(char *input, int *i)
{
	int		start;
	char	*var_name;
	int		len;

	start = *i;
	len = 0;
	while (input[*i] && is_valid_var_char(input[*i], len == 0))
	{
		(*i)++;
		len++;
	}
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (NULL);
	return (var_name);
}

char	*handle_question_var(t_minishell *shell)
{
	char	*exit_str;

	exit_str = ft_itoa(shell->last_exit_code);
	if (!exit_str)
		return (NULL);
	return (exit_str);
}

char	*get_var_value(char *var_name, t_minishell *shell)
{
	char	*value;
	int		i;

	if (ft_strncmp(var_name, "?", 1) == 0)
		return (handle_question_var(shell));
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var_name, ft_strlen(var_name)) == 0
			&& shell->env[i][ft_strlen(var_name)] == '=')
		{
			value = ft_strdup(shell->env[i] + ft_strlen(var_name) + 1);
			if (!value)
				return (NULL);
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}
