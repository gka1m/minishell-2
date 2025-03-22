/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:17:19 by zchan             #+#    #+#             */
/*   Updated: 2025/03/22 17:43:09 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

#define MAX_ENV_VAR_LENGTH 256

// Main function to set or update an environment variable in the list
// Check if the environment variable already exists, return 0 and update it
// If not found, create a new environment variable entry
// Add the new variable to the list
int	set_env_value(t_env **env_list, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_env;

	current = *env_list;
	while (current)
	{
		if (update_existing_env_var(current, key, value) == 0)
			return (0);
		current = current->next;
	}
	new_env = create_new_env_var(key, value);
	if (!new_env)
		return (-1);
	return (add_env_var_to_list(env_list, new_env));
}

// Function to add a key-value pair to the environment list
// Add the new variable to the list
// If the list is empty, make the new env variable the first element
// Wlse, while loop to traverse to the end of the list
// Append to the end of the list
int	add_key_value_env_var_to_list(t_env **env_list, const char *key,
		const char *value)
{
	t_env	*new_env;
	t_env	*current;

	new_env = create_new_env_var(key, value);
	if (!new_env)
		return (-1);
	current = *env_list;
	if (!current)
		*env_list = new_env;
	else
	{
		while (current->next)
			current = current->next;
		current->next = new_env;
	}
	return (0);
}

// Function to split the key-value pair from a string
// Split the environment variable by '='
// The first part is the key, the second part is the value
// Free the parts array, but keep the key and 
//  value since they are separately allocated
int	split_env_entry(const char *env_entry, char **key, char **value)
{
	char	**parts;

	parts = ft_split(env_entry, '=');
	if (!parts)
		return (-1);
	*key = parts[0];
	*value = parts[1];
	free(parts);
	return (0);
}

// Function to manually initialize the environment from a predefined list
// Split the current environment entry into key and value
// Add the key-value pair to the environment list
// Free the memory after adding to the list (or if in failure)
int	init_env_from_list(t_env **env_list)
{
	const char	*initial_env[] = {"HOME=/home/user", "PWD=/home/user",
			"OLDPWD=/home", "USER=example_user", NULL};
	int			i;
	char		*key;
	char		*value;

	i = 0;
	while (initial_env[i])
	{
		key = NULL;
		value = NULL;
		if (split_env_entry(initial_env[i], &key, &value) == -1)
			return (-1);
		if (add_key_value_env_var_to_list(env_list, key, value) == -1)
		{
			free(key);
			free(value);
			return (-1);
		}
		free(key);
		free(value);
		i++;
	}
	return (0);
}

// Function to simulate `getcwd` in custom environment
// Simulate updating the environment variables (e.g., OLDPWD and PWD)
int	update_oldpwd_and_pwd(t_minishell *minishell, t_env *env_list,
		char oldpwd[PATH_MAX])
{
	char	*home;

	home = get_env_value(env_list, "HOME");
	if (!home)
	{
		printf("cd: HOME not set\n");
		return (-1);
	}
	if (set_env_value(&env_list, "OLDPWD", oldpwd) == -1)
		return (-1);
	if (set_env_value(&env_list, "PWD", minishell->cwd) == -1)
		return (-1);
	return (0);
}

/* OLD LONG VERSION

// Function to set or update an environment variable in the list
// First check if the environment variable already exists
// Update it (return 0 if successful)
// If not found, create a new environment variable entry
// If the list is empty, set the new variable as the head
int	set_env_value(t_env **env_list, const char *key, const char *value)
{
	t_env	*current;
	t_env	*prev;
	t_env	*new_env;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
	{
		perror("malloc");
		return (-1);
	}
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = NULL;
	if (!*env_list)
		*env_list = new_env;
	else
		prev->next = new_env;
	return (0);
}

Old code that uses global variables

// Function to initialize environment from the global environment
int	init_env(t_env **env_list) {
	extern char **environ;  // To access the global environment variables
	int i = 0;

	while (environ[i]) {
		// Split the key and value from the "KEY=value" format
		char *env_entry = strdup(environ[i]);
		char *key = strtok(env_entry, "=");
		char *value = strtok(NULL, "=");

		if (set_env_value(env_list, key, value) == -1) {
			free(env_entry);
			return (-1);
		}

		free(env_entry);
		i++;
	}
	return (0);
}
*/