/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:30:39 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 13:12:44 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Delete an environment variable
void	delete_env_var(t_env **env, const char *key)
{
	t_env	*temp;
	t_env	*prev;

	temp = *env;
	prev = NULL;
	if (temp && ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
	{
		*env = temp->next;
		// free(temp->key);
		// free(temp->value);
		// free(temp);
		return ;
	}
	while (temp && ft_strncmp(temp->key, key, ft_strlen(key) + 1) != 0)
	{
		prev = temp;
		temp = temp->next;
	}
	if (!temp)
		return ;
	prev->next = temp->next;
	free(temp->key);
	free(temp->value);
	free(temp);
}
