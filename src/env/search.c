/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:29:45 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/01 14:23:10 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Find an environment variable in the list
t_env	*find_env_var(t_env *env, const char *key)
{
	size_t	keylen;

	keylen = ft_strlen(key);
	while (env)
	{
		if (ft_strlen(env->key) == keylen && ft_strncmp(env->key, key,
				keylen) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}
