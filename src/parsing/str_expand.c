/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:21:19 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/25 18:16:39 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_squote(char *input, int *i, char **result)
{
	int		start;
	char	*sub;

	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	sub = ft_substr(input, start, *i - start);
	if (!sub)
		return (0);
	*result = ft_strjoin_free(*result, sub);
	free(sub);
	if (!*result)
		return (0);
	if (input[*i] == '\'')
		(*i)++;
	return (1);
}

int	handle_dquote(char *input, int *i, t_minishell *shell, char **result)
{
	int		start;
	char	*sub;

	start = ++(*i);
	while (input[*i] && input[*i] != '\"')
	{
		if (input[*i] == '$')
		{
			sub = ft_substr(input, start, *i - start);
			if (!sub || !(*result = ft_strjoin_free(*result, sub)))
				return (free(sub), 0);
			free(sub);
			start = ++(*i);
			if (!expand_var(input, i, shell, result))
				return (0);
			start = *i;
		}
		else
			(*i)++;
	}
	sub = ft_substr(input, start, *i - start);
	if (!sub)
		return (0);
	*result = ft_strjoin_free(*result, sub);
	free(sub);
	if (!*result)
		return (0);
	if (input[*i] == '\"')
		(*i)++;
	return (1);
}

int	expand_var(char *input, int *i, t_minishell *shell, char **result)
{
	char	*var_name;
	char	*var_value;
	char	*tmp;

	if (input[*i] == '?')
	{
		var_name = ft_strdup("?");
		(*i)++;
	}
	else
		var_name = extract_var_name(input, i);
	if (!var_name)
		return (0);
	var_value = get_var_value(var_name, shell);
	free(var_name);
	if (!var_value)
		return (0);
	tmp = *result;
	*result = ft_strjoin(*result, var_value);
	free(var_value);
	free(tmp);
	if (!*result)
		return (0);
	return (1);
}

char	*expand_input(char *input, t_minishell *shell)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
		{
			if (!handle_squote(input, &i, &result))
				return (free(result), NULL);
		}
		else if (input[i] == '\"')
		{
			if (!handle_dquote(input, &i, shell, &result))
				return (free(result), NULL);
		}
		else if (input[i] == '$')
		{
			i++;
			if (!expand_var(input, &i, shell, &result))
				return (free(result), NULL);
		}
		else
		{
			result = ft_strjoin_char(result, input[i]);
			if (!result)
				return (NULL);
			i++;
		}
	}
	return (result);
}
