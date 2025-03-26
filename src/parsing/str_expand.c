/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:21:19 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/26 16:39:31 by kagoh            ###   ########.fr       */
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

// Handles variable expansion inside double quotes
int	handle_dquote_expansion(t_parse_ctx *ctx, int *start)
{
	char	*sub;

	sub = ft_substr(ctx->input, *start, *(ctx->i) - *start);
	*ctx->result = ft_strjoin_free(*ctx->result, sub);
	if (!sub || !(*ctx->result))
		return (free(sub), 0);
	free(sub);
	(*(ctx->i))++; // Move past '$'
	*start = *(ctx->i);
	if (!expand_var(ctx->input, ctx->i, ctx->shell, ctx->result))
		return (0);
	*start = *(ctx->i);
	return (1);
}

// Main function for handling double quotes
int	handle_dquote(char *input, int *i, t_minishell *shell, char **result)
{
	t_parse_ctx	ctx;
	int			start;
	char		*sub;

	ctx = (t_parse_ctx){input, i, shell, result};
	start = ++(*i);
	while (input[*i] && input[*i] != '\"')
	{
		if (input[*i] == '$')
		{
			if (!handle_dquote_expansion(&ctx, &start))
				return (0);
		}
		else
			(*i)++;
	}
	// Get the remaining part after last expansion
	sub = ft_substr(input, start, *i - start);
	*result = ft_strjoin_free(*result, sub);
	if (!sub || !(*result))
		return (free(sub), 0);
	free(sub);
	// Move past the closing quote
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

// Main function controlling the overall input expansion
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
		if (input[i] == '\'' || input[i] == '\"' || input[i] == '$')
		{
			if (!handle_specials(input, &i, shell, &result))
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
