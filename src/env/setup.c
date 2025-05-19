/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:08:13 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/19 11:38:48 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Initialize the environment from `envp`
t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;
	char	*key;
	char	*value;
	char	*sep;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		sep = ft_strchr(envp[i], '=');
		if (sep)
		{
			key = malloc(sep - envp[i] + 1);
			if (!key)
				return (free_env(env), NULL);
			ft_strlcpy(key, envp[i], sep - envp[i] + 1);
			value = ft_strdup(sep + 1);
			add_env_var(&env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (env);
}

// Update SHLVL environment variable
void	update_shlvl(t_env *env)
{
	t_env	*shlvl;
	int		level;
	char	*new_value;

	shlvl = find_env_var(env, "SHLVL");
	level = 1;
	if (shlvl && shlvl->value)
		level = ft_atoi(shlvl->value) + 1;
	new_value = ft_itoa(level);
	if (!new_value)
		return ;
	if (shlvl)
	{
		free(shlvl->value);
		shlvl->value = new_value;
	}
	else
	{
		add_env_var(&env, "SHLVL", new_value);
		free(new_value);
	}
}

// // Update an existing environment variable
// void	update_env_var(t_env *env, const char *key, const char *new_value)
// {
// 	t_env	*var;

// 	var = env;
// 	while (var)
// 	{
// 		if (strcmp(var->key, key) == 0)
// 		{
// 			free(var->value);
// 			var->value = strdup(new_value);
// 			return ;
// 		}
// 		var = var->next;
// 	}
// }

// int main(int argc, char **argv, char **envp)
// {
//     (void)argc;
//     (void)argv;

//     printf("=== Testing Environment Functions ===\n\n");

//     // 1. Test init_env() with system environment
//     printf("1. Initializing environment from envp:\n");
//     t_env *env = init_env(envp);
//     print_env_vars(env);
//     printf("\n");

//     // 2. Test update_shlvl()
//     printf("2. Updating SHLVL:\n");
//     update_shlvl(env);
//     t_env *shlvl = find_env_var(env, "SHLVL");
//     if (shlvl)
//         printf("Updated SHLVL=%s\n\n", shlvl->value);
//     else
//         printf("SHLVL not found\n\n");

//     // 3. Test add_env_var()
//     printf("3. Adding new variables:\n");
//     add_env_var(&env, "TEST_VAR", "hello_world");
//     add_env_var(&env, "ANOTHER_VAR", "42");
//     print_env_vars(env);
//     printf("\n");

//     // 4. Test find_env_var()
//     printf("4. Finding variables:\n");
//     t_env *found = find_env_var(env, "TEST_VAR");
//     if (found)
//         printf("Found TEST_VAR=%s\n", found->value);
//     else
//         printf("TEST_VAR not found\n");

//     found = find_env_var(env, "NON_EXISTENT");
//     if (found)
//         printf("Found NON_EXISTENT=%s\n", found->value);
//     else
//         printf("NON_EXISTENT not found\n\n");

//     // 5. Test update_env_var()
//     printf("5. Updating variables:\n");
//     update_env_var(env, "TEST_VAR", "updated_value");
//     found = find_env_var(env, "TEST_VAR");
//     if (found)
//         printf("Updated TEST_VAR=%s\n\n", found->value);
//     else
//         printf("TEST_VAR not found\n\n");

//     // 6. Test delete_env_var()
//     printf("6. Deleting variables:\n");
//     printf("Before deletion:\n");
//     print_env_vars(env);

//     delete_env_var(&env, "ANOTHER_VAR");
//     printf("\nAfter deleting ANOTHER_VAR:\n");
//     print_env_vars(env);

//     // 7. Test memory cleanup
//     printf("\n7. Testing memory cleanup...\n");
//     free_env(env);
//     printf("Environment list freed successfully.\n");

//     // 8. Test with empty environment
//     printf("\n8. Testing with empty environment:\n");
//     char *empty_env[] = {NULL};
//     env = init_env(empty_env);
//     printf("Empty environment initialized.\n");
//     print_env_vars(env);
//     free_env(env);

//     return (0);
// }
