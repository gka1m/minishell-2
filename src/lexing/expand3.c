/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:47:41 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 17:55:10 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// char	*expand_variables_with_quotes(char *str, t_minishell *shell)
// {
// 	size_t	i;
// 	char	*result;
// 	char	*temp;
// 	char	var_name[1024];
// 	t_env	*var;
// 	int		in_squote;
// 	int		in_dquote;
// 	size_t	j;
// 	char	*exit_status;
// 	char	ch[2];

// 	i = 0;
// 	result = ft_strdup("");
// 	in_squote = 0;
// 	in_dquote = 0;
// 	if (!str)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		// if (str[i] == '\'' && !in_dquote)
// 		// {
// 		// 	in_squote = !in_squote;
// 		// 	i++;
// 		// }
// 		// else if (str[i] == '"' && !in_squote)
// 		// {
// 		// 	in_dquote = !in_dquote;
// 		// 	i++;
// 		// }
// 		toggle_quote_state(str[i],&in_dquote, &in_squote);
// 		if ((str[i] == '\'' && !in_dquote) || (str[i] == '"' && !in_squote))
// 			i++;
// 		else if (str[i] == '$' && !in_squote)
// 			// Only expand $ outside single quotes
// 		{
// 			j = 0;
// 			i++;
// 			if (str[i] == '?') // Special case: $?
// 			{
// 				exit_status = ft_itoa(shell->last_exit_code);
// 				temp = ft_strjoin(result, exit_status);
// 				free(result);
// 				result = temp;
// 				free(exit_status);
// 				i++;
// 			}
// 			else if (ft_isalpha(str[i]) || str[i] == '_')
// 				// Valid variable start
// 			{
// 				while ((ft_isalnum(str[i]) || str[i] == '_')
// 					&& j < sizeof(var_name) - 1)
// 					var_name[j++] = str[i++];
// 				var_name[j] = '\0';
// 				var = find_env_var(shell->env_list, var_name);
// 				if (var && var->value)
// 				{
// 					temp = ft_strjoin(result, var->value);
// 					free(result);
// 					result = temp;
// 				}
// 			}
// 			else
// 			{
// 				// Just a standalone $, keep it
// 				temp = ft_strjoin(result, "$");
// 				free(result);
// 				result = temp;
// 			}
// 		}
// 		else
// 		{
// 			// Normal character, append
// 			ch[0] = str[i];
// 			ch[1] = 0;
// 			temp = ft_strjoin(result, ch);
// 			free(result);
// 			result = temp;
// 			i++;
// 		}
// 	}
// 	return (result);
// }

static void	handle_quote_state(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && !(*in_dquote))
		*in_squote = !(*in_squote);
	else if (c == '"' && !(*in_squote))
		*in_dquote = !(*in_dquote);
}

static void	charjoin(char **result, char c)
{
	char	ch[2];
	char	*temp;

	ch[0] = c;
	ch[1] = '\0';
	temp = ft_strjoin(*result, ch);
	free(*result);
	*result = temp;
}

static void	exp_var(char *str, size_t *i, char **result, t_minishell *shell)
{
	char	var_name[1024];
	size_t	j;
	t_env	*var;

	j = 0;
	while ((ft_isalnum(str[*i]) || str[*i] == '_') && j < sizeof(var_name) - 1)
		var_name[j++] = str[(*i)++];
	var_name[j] = '\0';
	var = find_env_var(shell->env_list, var_name);
	if (var && var->value)
		append_str(result, var->value);
}

static void	handle_dollar(char *str, size_t *i, char **result,
		t_minishell *shell)
{
	char	*status;

	(*i)++;
	if (str[*i] == '?')
	{
		status = ft_itoa(shell->last_exit_code);
		append_str(result, status);
		free(status);
		(*i)++;
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
		exp_var(str, i, result, shell);
	else
		charjoin(result, '$');
}

char	*expand_variables_with_quotes(char *str, t_minishell *shell)
{
	size_t	i;
	char	*result;
	int		in_squote;
	int		in_dquote;

	i = 0;
	result = ft_strdup("");
	in_squote = 0;
	in_dquote = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		handle_quote_state(str[i], &in_squote, &in_dquote);
		if ((str[i] == '\'' && !in_dquote) || (str[i] == '"' && !in_squote))
			i++;
		else if (str[i] == '$' && !in_squote)
			handle_dollar(str, &i, &result, shell);
		else
			charjoin(&result, str[i++]);
	}
	return (result);
}
