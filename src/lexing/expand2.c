/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:21:39 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 18:03:01 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	process_expansion_char(char *str, int *i, char **result,
		t_exp_context ctx)
{
	char	ch[2];

	ch[0] = str[*i];
	ch[1] = 0;
	if (str[*i] == '\'' && !(*ctx.in_dquote))
		handle_single_quote(i, result, ctx.in_squote);
	else if (str[*i] == '"' && !(*ctx.in_squote))
		handle_double_quote(i, result, ctx.in_dquote);
	else if (str[*i] == '$' && !(*ctx.in_squote) && str[*i + 1])
		handle_variable_expansion(str, i, result, ctx.shell);
	else
	{
		append_char(result, ch);
		(*i)++;
	}
}

void	handle_variable_expansion(char *str, int *i, char **result,
		t_minishell *shell)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_variable_name(str, i);
	if (!var_name)
		return ;
	if (var_name[0] == '\0')
	{
		append_char(result, "$");
		free(var_name);
		return ;
	}
	var_value = get_variable_value(shell, var_name);
	if (!var_value)
		var_value = ft_strdup("");
	append_str(result, var_value);
	free(var_name);
	free(var_value);
}

// void	handle_variable_expansion(char *str, int *i, char **result,
		// t_minishell *shell)
// {
// 	char	*var_name;
// 	char	*var_value;
// 	int		start = *i;

// 	var_name = extract_variable_name(str, i);
// 	if (!var_name)
// 	{
// 		append_char(result, "$");
// 		return ;
// 	}

// 	var_value = get_variable_value(shell, var_name);
// 	if (!var_value)
// 	{
// 		// If variable is empty/undefined,
			// we still want to keep the original $VAR
// 		// So we append the original text including the $ and variable name
// 		append_str(result, ft_substr(str, start, *i - start));
// 	}
// 	else
// 	{
// 		append_str(result, var_value);
// 		free(var_value);
// 	}
// 	free(var_name);
// }

char	*extract_variable_name(char *str, int *i)
{
	int	start;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*get_variable_value(t_minishell *shell, char *var_name)
{
	t_env	*env_var;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(shell->last_exit_code));
	env_var = find_env_var(shell->env_list, var_name);
	if (env_var && env_var->value)
		return (ft_strdup(env_var->value));
	return (NULL);
}

void	expand_variables(t_token *token, t_minishell *shell)
{
	char	*result;
	int		i;
	int		in_squote;
	int		in_dquote;

	if (!token || token->type != T_STRING || !token->value)
		return ;
	result = ft_strdup("");
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (token->value[i])
		process_expansion_char(token->value, &i, &result, (t_exp_context){shell,
			&in_squote, &in_dquote});
	free(token->value);
	token->value = result;
}
