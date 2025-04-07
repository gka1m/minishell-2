/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:30:52 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/07 12:37:49 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*remove_quotes(char *str)
{
	if (!str || ft_strlen(str) < 2)
		return (ft_strdup(str));
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\'') || (str[0] == '"'
			&& str[ft_strlen(str) - 1] == '"'))
		return (ft_substr(str, 1, ft_strlen(str) - 2));
	return (ft_strdup(str));
}

char	*expand_heredoc_line(char *line, t_minishell *shell)
{
	char *result;
	char *buf;
	char *src = line;
	char *dst;
	char var_name[256];
	t_env *env_var;

	if (!(ft_strchr(line, '$')))
		return (ft_strdup(line));
	buf = ft_calloc(ft_strlen(line) * 2 + 1, sizeof(char));
	if (!buf)
		return (NULL);
	dst = buf;
	while (*src)
	{
		if (*src == '$' && (ft_isalnum(src[1]) || src[1] == '_'))
		{
			// Extract variable name
			char *v = var_name;
			src++; // Skip $
			while (ft_isalnum(*src) || *src == '_')
			{
				*v++ = *src++;
			}
			*v = '\0';
			// Find variable in environment
			env_var = find_env_var(shell->env_list, var_name);
			if (env_var && env_var->value)
			{
				ft_strlcpy(dst, env_var->value, ft_strlen(env_var->value) + 1);
				dst += ft_strlen(env_var->value);
			}
		}
		else
			*dst++ = *src++;
	}
    result = ft_strdup(buf);
    free(buf);
    return (result);
}
