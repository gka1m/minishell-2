/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkaim <gkaim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:43:54 by gkaim             #+#    #+#             */
/*   Updated: 2025/03/21 16:09:46 by gkaim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char    *get_env_val(char **env, char *key)
{
    size_t  key_len;
    int     i;

    key_len = ft_strlen(key);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
            return (env[i] + key_len + 1); // Return the value part of the env entry
        i++;
    }
    return (NULL); // Key not found
}

char    *extract_var(char **str)
{
    char    *temp;
    int     i;
    int     j;

    i = 0;
    while (ft_isalnum((*str)[i]) || (*str)[i] == '_')
        i++;
    temp = malloc((i + 1) * sizeof(char));
    if (!temp)
        return (NULL);
    j = 0;
    while (j++ < i)
        temp[j] = (*str)[j];
    temp[j] = '\0';
    (*str) += i;
    return (temp);
}

void    handle_exit(char **str, char **result, t_minishell *shell)
{
    char    *temp;
    char    *var;

    temp = *result;
    var = ft_itoa(shell->last_exit_code); // Use last_exit_code from t_minishell
    *result = ft_strjoin(temp, var);
    (free(temp), free(var));
    (*str)++;
}

void    var_value(char **str, char **result, t_minishell *shell)
{
    char    *temp;
    char    *tmp;
    char    *var;

    var = NULL;
    temp = extract_var(str); // Extract the variable name
    var = get_env_val(shell->env, temp); // Look up the variable in shell->env
    free(temp);
    temp = *result;
    if (!var)
        *result = ft_strjoin(temp, ""); // If the variable is not found, append an empty string
    else if (check_quotes(var))
    {
        tmp = affix_quote(var); // Add quotes if necessary
        *result = ft_strjoin(temp, tmp);
        free(tmp);
    }
    else
        *result = ft_strjoin(temp, var); // Append the variable value
    if (temp)
        free(temp);
}

void    handle_var(char **str, char **result, t_minishell *shell)
{
    (*str)++;
    if (!**str)
    {
        *result = append_char('$', *result); // Append '$'
        *result = append_char(**str, *result); // Append the current character
        return ;
    }
    if (**str == '?')
        handle_exit(str, result, shell); // Handle the special case for $?
    else if (!ft_isalnum(**str) && (**str) != '_')
    {
        *result = append_char('$', *result); // Append '$'
        *result = append_char(**str, *result); // Append the current character
        (*str)++;
    }
    else
        var_value(str, result, shell); // Handle regular environment variables
}
