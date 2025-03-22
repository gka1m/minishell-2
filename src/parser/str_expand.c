/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkaim <gkaim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:27:12 by gkaim             #+#    #+#             */
/*   Updated: 2025/03/21 15:41:04 by gkaim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char    *single_quote(char *str, char *result)
{
    result = append_char(*str, result); // Append the opening quote
    str++;
    while (*str && *str != '\'')
    {
        result = append_char(*str, result); // Append characters inside quotes
        str++;
    }
    if (*str)
    {
        result = append_char(*str, result); // Append the closing quote
        str++;
    }
    return (result);
}

char    *double_quote(char *str, char *result, t_minishell *shell)
{
    result = append_char(*str, result); // Append the opening quote
    str++;
    while (*str && *str != '"')
    {
        if (*str == '$')
            result = handle_var(str, result, shell); // Handle variable expansion
        else
        {
            result = append_char(*str, result); // Append characters inside quotes
            str++;
        }
    }
    if (*str)
    {
        result = append_char(*str, result); // Append the closing quote
        str++;
    }
    return (result);
}

char    *cmd_expand(char *str, t_minishell *shell)
{
    char    *result;

    result = ft_strdup("");
    if (!result)
        return (NULL);
    while (str && *str)
    {
        if (*str == '\'')
            result = handle_quote(str, result);
        else if (*str == '"')
            result = handle_dquote(str, result, shell);
        else if (*str == '$')
            result = handle_var(str, result, shell);
        else
        {
            result = append_char(*str, result); // Append regular characters
            str++;
        }
        if (!result) // Check for memory allocation failure
            return (NULL);
    }
    result = append_char('\0', result); // Null-terminate the result
    return (result);
}

char	*append_char(char c, char *str)
{
	char	*new;
	size_t	len;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	new = malloc((len + 2) * sizeof(char));
	if (!new)
		return (NULL);
	ft_memcpy(new, str, len);
	new[len] = c;
	new[len + 1] = '\0';
	return (new);
}