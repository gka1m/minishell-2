/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:38:34 by zchan             #+#    #+#             */
/*   Updated: 2025/03/22 17:43:20 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

#define MAX_ENV_VAR_LENGTH 256

// Function to find an environment variable in the list
char	*get_env_value(t_env *env_list, const char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

// Function to check if an environment variable already exists and update it
int	update_existing_env_var(t_env *current, const char *key, const char *value)
{
	if (ft_strcmp(current->key, key) == 0)
	{
		free(current->value);
		current->value = strdup(value);
		return (0);
	}
	return (-1);
}

// Function to create a new environment variable
t_env	*create_new_env_var(const char *key, const char *value)
{
	t_env	*new_env;

	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
	{
		perror("malloc");
		return (NULL);
	}
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = NULL;
	return (new_env);
}

// Function to add a new environment variable to the list
int	add_env_var_to_list(t_env **env_list, t_env *new_env)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	if (!*env_list)
		*env_list = new_env;
	else
	{
		while (current)
		{
			prev = current;
			current = current->next;
		}
		prev->next = new_env;
	}
	return (0);
}

// Function to free the environment list
void	free_env(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}
