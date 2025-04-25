/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:49:27 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/25 12:55:43 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Processes a single export argument (VAR or VAR=value) */
int	process_export_arg(t_env **env, char *arg)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	t_env	*existing;

	if (!arg)
		return (1);
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		// Extract key and value
		key = ft_strndup(arg, equal_sign - arg);
		if (!key)
			return (1);
		value = ft_strdup(equal_sign + 1);
		if (!value)
			return (1);
		// First try to update existing variable
		existing = find_env_var(*env, key);
		if (existing)
		{
			free(existing->value);
			existing->value = value;
			free(key);
		}
		else
		{
			// Add new variable
			add_env_var(env, key, value);
			free(key);
			free(value); // add_env_var should make its own copies
		}
	}
	else
	{
		key = ft_strdup(arg);
		if (!key)
			return (1);
		// Just ensure the variable exists with NULL value if it doesn't
		if (!find_env_var(*env, arg))
			add_env_var(env, arg, NULL);
	}
	return (0);
}

// int process_export_arg(t_env **env, char *arg)
// {
//     char *equal_sign;
//     char *key;
//     char *value;
//     t_env *existing;

//     if (!arg || !*arg) // Handle empty argument
//         return 1;

//     equal_sign = ft_strchr(arg, '=');
//     if (equal_sign)
//     {
//         // Extract key
//         key = ft_strndup(arg, equal_sign - arg);
//         if (!key)
//             return 1;

//         // Extract value - handle empty cases properly
//         if (*(equal_sign + 1) == '\0') {
//             value = ft_strdup(""); // Empty string after =
//         } else {
//             value = ft_strdup(equal_sign + 1);
//         }

//         if (!value) {
//             free(key);
//             return 1;
//         }

//         // Update or add the variable
//         existing = find_env_var(*env, key);
//         if (existing)
//         {
//             free(existing->value);
//             existing->value = value;
//             free(key);
//         }
//         else
//         {
//             add_env_var(env, key, value); // add_env_var should duplicate the value
//             free(value);
//             free(key);
//         }
//     }
//     else
//     {
//         // Just ensure the variable exists with NULL value if it doesn't
//         key = ft_strdup(arg);
//         if (!key)
//             return 1;
            
//         if (!find_env_var(*env, key))
//             add_env_var(env, key, NULL);
//         else
//             free(key);
//     }
//     return 0;
// }

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	char	*new_str;

	if (!s)
		return (NULL);
	// Calculate the actual length to copy (whichever is smaller: n or strlen(s))
	len = ft_strlen(s);
	if (len > n)
		len = n;
	// Allocate memory (len + 1 for null terminator)
	new_str = (char *)malloc(len + 1);
	if (!new_str)
		return (NULL);
	// Copy the string and null-terminate
	ft_memcpy(new_str, s, len);
	new_str[len] = '\0';
	return (new_str);
}

/* Main export builtin function */
// int	bi_export(t_minishell *shell, char **args, int fd_out)
// {
// 	int	i;
// 	int	ret;

// 	(void)fd_out; // For future redirection support
// 	if (!args[1])
// 	{
// 		print_sorted_env(shell->env_list, STDOUT_FILENO);
// 		return (EXIT_SUCCESS);
// 	}
// 	ret = EXIT_SUCCESS;
// 	i = 1;
// 	while (args[i])
// 	{
// 		if (!is_valid_env_name(args[i]))
// 		{
// 			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
// 			ft_putstr_fd(args[i], STDERR_FILENO);
// 			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
// 			ret = EXIT_FAILURE;
// 		}
// 		else if (process_export_arg(&shell->env_list, args[i]))
// 			ret = EXIT_FAILURE;
// 		i++;
// 	}
// 	return (ret);
// }

int	bi_export(t_minishell *shell, char **args, int fd_out)
{
	int		i;
	int		ret;
	char	*key;
	char	*eq;

	(void)fd_out;
	if (!args[1])
	{
		print_sorted_env(shell->env_list, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	ret = EXIT_SUCCESS;
	i = 1;
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
			key = ft_strndup(args[i], eq - args[i]);
		else
			key = ft_strdup(args[i]);
		if (!is_valid_env_name(key))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = EXIT_FAILURE;
		}
		else if (process_export_arg(&shell->env_list, args[i]))
			ret = EXIT_FAILURE;
		free(key);
		i++;
	}
	return (ret);
}

