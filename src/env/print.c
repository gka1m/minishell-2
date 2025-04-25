/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:30:13 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/25 16:20:16 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// void	print_env_vars(t_env *env)
// {
// 	while (env)
// 	{
// 		if (env->value != NULL)
// 			printf("%s=%s\n", env->key, env->value);
// 		env = env->next;
// 	}
// }

void	print_env_vars(t_env *env)
{
	while (env)
	{
		if (env->value && strcmp(env->value, "##NO_VALUE##") != 0)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
