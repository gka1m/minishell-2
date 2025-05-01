/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:57:25 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/01 14:17:20 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// char	*remove_quotes(char *str)
// {
// 	size_t	len;
// 	char	*result;
// 	int		in_outer_quotes;
// 	char	outer_quote_char;
// 	size_t	i;
// 	int		in_squote;
// 	int		in_dquote;
// 	char	*temp;

// 	in_outer_quotes = 0;
// 	outer_quote_char = 0;
// 	i = 0;
// 	in_squote = 0;
// 	in_dquote = 0;
// 	if (!str)
// 		return (NULL);
// 	len = ft_strlen(str);
// 	if (len < 2)
// 		return (ft_strdup(str));
// 	result = ft_strdup("");
// 	if (!result)
// 		return (NULL);
// 	// Check outer quotes
// 	if ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"' && str[len
// 			- 1] == '"'))
// 	{
// 		outer_quote_char = str[0];
// 		in_outer_quotes = 1;
// 		i = 1;
// 		len--;
// 	}
// 	while (i < len)
// 	{
// 		char ch[2] = {str[i], 0}; // Moved inside loop
// 		if (str[i] == '\'' && !in_dquote)
// 		{
// 			in_squote = !in_squote;
// 			if (!in_outer_quotes || outer_quote_char != '\'')
// 			{
// 				temp = ft_strjoin(result, ch);
// 				free(result);
// 				result = temp;
// 			}
// 			i++;
// 		}
// 		else if (str[i] == '"' && !in_squote)
// 		{
// 			in_dquote = !in_dquote;
// 			if (!in_outer_quotes || outer_quote_char != '"')
// 			{
// 				temp = ft_strjoin(result, ch);
// 				free(result);
// 				result = temp;
// 			}
// 			i++;
// 		}
// 		else
// 		{
// 			temp = ft_strjoin(result, ch);
// 			free(result);
// 			result = temp;
// 			i++;
// 		}
// 	}
// 	return (result);
// }

char	*remove_quotes(char *str)
{
	char	*result;
	char	*temp;
	int		in_squote;
	int		in_dquote;
	size_t	i;
	char	ch[2];

	result = ft_strdup("");
	in_squote = 0;
	in_dquote = 0;
	i = 0;
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
		else
		{
			ch[0] = str[i];
			ch[1] = '\0';
			temp = ft_strjoin(result, ch);
			free(result);
			result = temp;
			i++;
		}
	}
	return (result);
}

char *remove_outer_quotes(char *str)
{
    size_t  len;
    char    *result;

    if (!str)
        return NULL;
    
    len = ft_strlen(str);
    if (len >= 2 && 
        ((str[0] == '\'' && str[len-1] == '\'') || 
         (str[0] == '"' && str[len-1] == '"')))
    {
        result = ft_substr(str, 1, len - 2);
    }
    else
    {
        result = ft_strdup(str);
    }
    
    // Ensure we never return NULL
    if (!result)
        result = ft_strdup("");
    
    return result;
}

// Fixed expand_variables
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

// 	if (!token || token->type != T_STRING || !token->value)
// 		return ;
// 	result = ft_strdup("");
// 	i = 0;
// 	in_squote = 0;
// 	in_dquote = 0;
// 	while (token->value[i])
// 	{
// 		char ch[2] = {token->value[i], 0}; // Moved INSIDE loop
// 		if (token->value[i] == '\'' && !in_dquote)
// 		{
// 			in_squote = !in_squote;
// 			temp = ft_strjoin(result, "'");
// 			free(result);
// 			result = temp ? temp : result;
// 			i++;
// 		}
// 		else if (token->value[i] == '"' && !in_squote)
// 		{
// 			in_dquote = !in_dquote;
// 			temp = ft_strjoin(result, "\"");
// 			free(result);
// 			result = temp ? temp : result;
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
// 				{
// 					var_value = ft_itoa(shell->last_exit_code);
// 				}
// 				else
// 				{
// 					env_var = find_env_var(shell->env_list, var_name);
// 					var_value = env_var ? ft_strdup(env_var->value) : ft_strdup("");
// 				}
// 				if (var_value)
// 				{
// 					temp = ft_strjoin(result, var_value);
// 					free(result);
// 					result = temp ? temp : result;
// 					free(var_value);
// 				}
// 				free(var_name);
// 			}
// 		}
// 		else
// 		{
// 			temp = ft_strjoin(result, ch);
// 			free(result);
// 			result = temp ? temp : result;
// 			i++; // CRITICAL: Was missing in your code
// 		}
// 	}
// 	free(token->value);
// 	token->value = result;
// }

void	expand_variables(t_token *token, t_minishell *shell)
{
	char	*result;
	int		i;
	int		in_squote;
	int		in_dquote;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		start;
	t_env	*env_var;
	char	ch[2];
		

	if (!token || token->type != T_STRING || !token->value)
		return ;
	result = ft_strdup("");
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (token->value[i])
	{
		// char ch[2] = {token->value[i], 0};
		ch[0] = token->value[i];
		ch[1] = 0;
		if (token->value[i] == '\'' && !in_dquote)
		{
			in_squote = !in_squote;
			temp = ft_strjoin(result, "'");
			free(result);
			if (temp)
				result = temp;
			i++;
		}
		else if (token->value[i] == '"' && !in_squote)
		{
			in_dquote = !in_dquote;
			temp = ft_strjoin(result, "\"");
			free(result);
			if (temp)
				result = temp;
			i++;
		}
		else if (token->value[i] == '$' && !in_squote && token->value[i + 1])
		{
			i++;
			var_name = NULL;
			var_value = NULL;
			if (token->value[i] == '?')
			{
				var_name = ft_strdup("?");
				i++;
			}
			else
			{
				start = i;
				while (token->value[i] && (ft_isalnum(token->value[i])
						|| token->value[i] == '_'))
					i++;
				var_name = ft_substr(token->value, start, i - start);
			}
			if (var_name)
			{
				if (ft_strncmp(var_name, "?", 1) == 0)
					var_value = ft_itoa(shell->last_exit_code);
				else
				{
					env_var = find_env_var(shell->env_list, var_name);
					if (env_var)
						var_value = ft_strdup(env_var->value);
					else
						var_value = ft_strdup("");
				}
				if (var_value)
				{
					temp = ft_strjoin(result, var_value);
					free(result);
					if (temp)
						result = temp;
					free(var_value);
				}
				free(var_name);
			}
		}
		else
		{
			temp = ft_strjoin(result, ch);
			free(result);
			if (temp)
				result = temp;
			i++;
		}
	}
	free(token->value);
	token->value = result;
}

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


// Fixed expand_all_tokens
t_token *expand_all_tokens(t_token *tokens, t_minishell *shell)
{
    t_token *current;
    char *expanded;
    char *unquoted;

    current = tokens;
    while (current)
    {
        if (is_redirection(current))
        {
            current = handle_redirection_expansion(current, shell);
            if (current)
                current = current->next;
        }
        else if (current->type == T_STRING)
        {
            if (current->previous && current->previous->type == T_HEREDOC)
            {
                // Heredoc: no variable expansion, only remove quotes
                unquoted = remove_outer_quotes(current->value);
                free(current->value);
                current->value = unquoted;
            }
            else
            {
                expanded = expand_variables_with_quotes(current->value, shell);
                free(current->value);
                current->value = expanded;

                unquoted = remove_outer_quotes(current->value);
                free(current->value);
				// free(expanded);
                current->value = unquoted;
            }
            current = current->next;
        }
        else
            current = current->next;
    }
    // Only AFTER expansion and unquoting
    tokens = concatenate_adjacent_strings(tokens);
    return (tokens);
}


// Simplified redirection handler
t_token	*handle_redirection_expansion(t_token *token, t_minishell *shell)
{
	t_token	*file_token;
	char	*unquoted;

	if (!token || !is_redirection(token))
		return (token);
	file_token = token->next;
	if (!file_token || file_token->type != T_STRING)
		return (token);
	expand_variables(file_token, shell);
	unquoted = remove_quotes(file_token->value);
	free(file_token->value);
	file_token->value = unquoted;
	return (token);
}

t_token	*concatenate_adjacent_strings(t_token *tokens)
{
	t_token	*curr;
	t_token	*next;
	char	*temp;
	char	*curr_unquoted;
	char	*next_unquoted;

	curr = tokens;
	while (curr && curr->next)
	{
		next = curr->next;
		// Only merge adjacent quoted strings
		if (curr->type == T_STRING && next->type == T_STRING && next->adjacent
			&& (curr->value[0] == '"' || curr->value[0] == '\'')
				&& (next->value[0] == '"' || next->value[0] == '\''))
		{
			curr_unquoted = remove_quotes(curr->value);
			next_unquoted = remove_quotes(next->value);
			temp = ft_strjoin(curr_unquoted, next_unquoted);
			free(curr->value);
			curr->value = temp;
			free(curr_unquoted);
			free(next_unquoted);
			curr->next = next->next;
			free(next->value);
			free(next);
		}
		else
			curr = curr->next;
	}
	return (tokens);
}
