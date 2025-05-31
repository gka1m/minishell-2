/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:18:53 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 15:21:19 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_quoted_string(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	return (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '"' && str[len - 1] == '"')));
}

char	*remove_delim_q(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (is_quoted_string(str))
		return (ft_substr(str, 1, len - 2));
	else
		return (ft_strdup(str));
}

char	**append_arg(char **args, char *new_arg)
{
	size_t	len;
	size_t	i;
	char	**new_args;

	len = 0;
	i = 0;
	while (args && args[len])
		len++;
	new_args = malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return (NULL);
	while (i < len)
	{
		new_args[i] = ft_strdup(args[i]);
		i++;
	}
	new_args[len] = ft_strdup(new_arg);
	new_args[len + 1] = NULL;
	if (args)
		free_split(args);
	return (new_args);
}
