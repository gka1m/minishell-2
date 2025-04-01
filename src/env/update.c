/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:28:34 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/01 12:28:55 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Update an environment variable's value
void	update_env_var(t_env *env, const char *key, const char *new_value)
{
	t_env	*var;

	var = find_env_var(env, key);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(new_value);
	}
}
