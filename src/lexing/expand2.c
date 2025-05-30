/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:21:39 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/30 15:38:46 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	append_char(char **result, char *ch)
{
	char	*temp = ft_strjoin(*result, ch);
	free(*result);
	if (temp)
		*result = temp;
}

void	append_str(char **result, char *str)
{
	char	*temp = ft_strjoin(*result, str);
	free(*result);
	if (temp)
		*result = temp;
}

void	process_expansion_char(char *str, int *i, char **result, t_exp_context ctx)
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

void	handle_single_quote(int *i, char **result, int *in_squote)
{
	*in_squote = !(*in_squote);
	append_char(result, "'");
	(*i)++;
}

void	handle_double_quote(int *i, char **result, int *in_dquote)
{
	*in_dquote = !(*in_dquote);
	append_char(result, "\"");
	(*i)++;
}

void	handle_variable_expansion(char *str, int *i, char **result, t_minishell *shell)
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
		return;
	}
	var_value = get_variable_value(shell, var_name);
	if (!var_value)
		var_value = ft_strdup("");
	append_str(result, var_value);
	free(var_name);
	free(var_value);
}

// void	handle_variable_expansion(char *str, int *i, char **result, t_minishell *shell)
// {
// 	char	*var_name;
// 	char	*var_value;
// 	int		start = *i;

// 	var_name = extract_variable_name(str, i);
// 	if (!var_name)
// 	{
// 		append_char(result, "$");
// 		return;
// 	}
	
// 	var_value = get_variable_value(shell, var_name);
// 	if (!var_value)
// 	{
// 		// If variable is empty/undefined, we still want to keep the original $VAR
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
	int		start;

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
		process_expansion_char(token->value, &i, &result,
			(t_exp_context){shell, &in_squote, &in_dquote});
	free(token->value);
	token->value = result;
}

// void	expand_variables(t_token *token, t_minishell *shell)
// {
// 	char	*result;
// 	int		i;
// 	int		in_squote;
// 	int		in_dquote;
// 	char	*temp;
// 	char	*var_name;
// 	char	*var_value;
// 	int		start;
// 	t_env	*env_var;
// 	char	ch[2];
		

// 	if (!token || token->type != T_STRING || !token->value)
// 		return ;
// 	result = ft_strdup("");
// 	i = 0;
// 	in_squote = 0;
// 	in_dquote = 0;
// 	while (token->value[i])
// 	{
// 		// char ch[2] = {token->value[i], 0};
// 		ch[0] = token->value[i];
// 		ch[1] = 0;
// 		if (token->value[i] == '\'' && !in_dquote)
// 		{
// 			in_squote = !in_squote;
// 			temp = ft_strjoin(result, "'");
// 			free(result);
// 			if (temp)
// 				result = temp;
// 			i++;
// 		}
// 		else if (token->value[i] == '"' && !in_squote)
// 		{
// 			in_dquote = !in_dquote;
// 			temp = ft_strjoin(result, "\"");
// 			free(result);
// 			if (temp)
// 				result = temp;
// 			i++;
// 		}
// 		else if (token->value[i] == '$' && !in_squote && token->value[i + 1])
// 		{
// 			i++;
// 			var_name = NULL;
// 			var_value = NULL;
// 			if (token->value[i] == '?')
// 			{
// 				var_name = ft_strdup("?");
// 				i++;
// 			}
// 			else
// 			{
// 				start = i;
// 				while (token->value[i] && (ft_isalnum(token->value[i])
// 						|| token->value[i] == '_'))
// 					i++;
// 				var_name = ft_substr(token->value, start, i - start);
// 			}
// 			if (var_name)
// 			{
// 				if (ft_strncmp(var_name, "?", 1) == 0)
// 					var_value = ft_itoa(shell->last_exit_code);
// 				else
// 				{
// 					env_var = find_env_var(shell->env_list, var_name);
// 					if (env_var)
// 						var_value = ft_strdup(env_var->value);
// 					else
// 						var_value = ft_strdup("");
// 				}
// 				if (var_value)
// 				{
// 					temp = ft_strjoin(result, var_value);
// 					free(result);
// 					if (temp)
// 						result = temp;
// 					free(var_value);
// 				}
// 				free(var_name);
// 			}
// 		}
// 		else
// 		{
// 			temp = ft_strjoin(result, ch);
// 			free(result);
// 			if (temp)
// 				result = temp;
// 			i++;
// 		}
// 	}
// 	free(token->value);
// 	token->value = result;
// }

// void	append_char_to_result(char **result, char c)
// {
// 	char	ch[2];
// 	char	*temp;

// 	ch[0] = c;
// 	ch[1] = '\0';
// 	temp = ft_strjoin(*result, ch);
// 	free(*result);
// 	*result = temp;
// }

// void	handle_exit_status(char **result, t_minishell *shell)
// {
// 	char	*exit_status;
// 	char	*temp;

// 	exit_status = ft_itoa(shell->last_exit_code);
// 	temp = ft_strjoin(*result, exit_status);
// 	free(*result);
// 	*result = temp;
// 	free(exit_status);
// }

// void	handle_variable_name(char *str, size_t *i, char **result, t_minishell *shell)
// {
// 	char	var_name[1024];
// 	size_t	j;
// 	t_env	*var;
// 	char	*temp;

// 	j = 0;
// 	while ((ft_isalnum(str[*i]) || str[*i] == '_') && j < sizeof(var_name) - 1)
// 	{
// 		var_name[j] = str[*i];
// 		j++;
// 		(*i)++;
// 	}
// 	var_name[j] = '\0';
// 	var = find_env_var(shell->env_list, var_name);
// 	if (var && var->value)
// 	{
// 		temp = ft_strjoin(*result, var->value);
// 		free(*result);
// 		*result = temp;
// 	}
// }

// void	handle_variable_expansion_in_str(char *str, size_t *i, char **result, t_minishell *shell)
// {
// 	(*i)++;
// 	if (str[*i] == '?')
// 	{
// 		handle_exit_status(result, shell);
// 		(*i)++;
// 	}
// 	else if (ft_isalpha(str[*i]) || str[*i] == '_')
// 		handle_variable_name(str, i, result, shell);
// 	else
// 	{
// 		append_char_to_result(result, '$');
// 	}
// }

// char	*expand_variables_with_quotes(char *str, t_minishell *shell)
// {
// 	size_t	i;
// 	char	*result;
// 	int		in_squote;
// 	int		in_dquote;

// 	if (!str)
// 		return (NULL);
// 	result = ft_strdup("");
// 	i = 0;
// 	in_squote = 0;
// 	in_dquote = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '\'' && !in_dquote)
// 			in_squote = !in_squote;
// 		else if (str[i] == '"' && !in_squote)
// 			in_dquote = !in_dquote;
// 		else if (str[i] == '$' && !in_squote)
// 			handle_variable_expansion_in_str(str, &i, &result, shell);
// 		else
// 			append_char_to_result(&result, str[i]);
// 		i++;
// 	}
// 	return (result);
// }

char *expand_variables_with_quotes(char *str, t_minishell *shell)
{
    size_t i = 0;
    char *result = ft_strdup("");
    char *temp;
    char var_name[1024];
    t_env *var;
    int in_squote = 0;
    int in_dquote = 0;

    if (!str)
        return (NULL);

    while (str[i])
    {
        if (str[i] == '\'' && !in_dquote)
        {
            in_squote = !in_squote;
            i++;
        }
        else if (str[i] == '"' && !in_squote)
        {
            in_dquote = !in_dquote;
            i++;
        }
        else if (str[i] == '$' && !in_squote) // Only expand $ outside single quotes
        {
            size_t j = 0;
            i++;
            if (str[i] == '?') // Special case: $?
            {
                char *exit_status = ft_itoa(shell->last_exit_code);
                temp = ft_strjoin(result, exit_status);
                free(result);
                result = temp;
                free(exit_status);
                i++;
            }
            else if (ft_isalpha(str[i]) || str[i] == '_') // Valid variable start
            {
                while ((ft_isalnum(str[i]) || str[i] == '_') && j < sizeof(var_name) - 1)
                    var_name[j++] = str[i++];
                var_name[j] = '\0';
                var = find_env_var(shell->env_list, var_name);
                if (var && var->value)
                {
                    temp = ft_strjoin(result, var->value);
                    free(result);
                    result = temp;
                }
            }
            else
            {
                // Just a standalone $, keep it
                temp = ft_strjoin(result, "$");
                free(result);
                result = temp;
            }
        }
        else
        {
            // Normal character, append
            char ch[2] = {str[i], '\0'};
            temp = ft_strjoin(result, ch);
            free(result);
            result = temp;
            i++;
        }
    }
    return (result);
}
