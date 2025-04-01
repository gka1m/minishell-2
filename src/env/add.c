/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:26:46 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/01 12:27:26 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Create a new environment variable node
t_env	*new_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

// Add an environment variable to the linked list
void	add_env_var(t_env **env, const char *key, const char *value)
{
	t_env	*new;
	t_env	*temp;

	new = new_env_node(key, value);
	if (!new)
		return ;
	if (!*env)
		*env = new;
	else
	{
		temp = *env;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}
