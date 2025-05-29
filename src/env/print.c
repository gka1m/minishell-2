/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:30:13 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/29 09:36:13 by kagoh            ###   ########.fr       */
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
		if (env->value && ft_strcmp(env->value, "##NO_VALUE##") != 0)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
