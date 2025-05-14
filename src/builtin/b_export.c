/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:49:27 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/14 16:26:59 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_empty_assignment(const char *arg)
{
	const char	*eq;

	eq = ft_strchr(arg, '=');
	return (eq && (*(eq + 1) == '\0' || (*(eq + 1) == '"' && *(eq
					+ 2) == '"')));
}

/* Processes a single export argument (VAR or VAR=value) */
// int	process_export_arg(t_env **env, char *arg)
// {
// 	char	*equal_sign;
// 	char	*key;
// 	char	*value;
// 	t_env	*existing;

// 	if (!arg)
// 		return (1);
// 	equal_sign = ft_strchr(arg, '=');
// 	if (equal_sign)
// 	{
// 		// Extract key and value
// 		key = ft_strndup(arg, equal_sign - arg);
// 		if (!key)
// 			return (1);
// 		value = ft_strdup(equal_sign + 1);
// 		if (!value)
// 			return (free(key), 1);
// 		// First try to update existing variable
// 		existing = find_env_var(*env, key);
// 		if (existing)
// 		{
// 			free(existing->value);
// 			existing->value = value;
// 			free(key);
// 		}
// 		else
// 		{
// 			// Add new variable
// 			add_env_var(env, key, value);
// 			free(key);
// 			free(value); // add_env_var should make its own copies
// 		}
// 	}
// 	else
// 	{
// 		key = ft_strdup(arg);
// 		if (!key)
// 			return (1);
// 		existing = find_env_var(*env, key);
// 		if (!existing)
// 			add_env_var(env, key, NULL);
// 		// If it already exists, do nothing (don't overwrite PATH etc)
// 		free(key);
// 	}
// 	return (0);
// }

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
		key = ft_strndup(arg, equal_sign - arg);
		if (!key)
			return (1);
		if (is_empty_assignment(arg))
			value = ft_strdup("");
		else
			value = ft_strdup(equal_sign + 1);
		if (!value)
			return (free(key), 1);
		existing = find_env_var(*env, key);
		if (existing)
		{
			(free(existing->value), free(key));
			existing->value = value;
		}
		else
		{
			add_env_var(env, key, value);
			(free(key), free(value));
		}
	}
	else
	{
		key = ft_strdup(arg);
		if (!key)
			return (1);
		existing = find_env_var(*env, arg);
		if (!existing)
			add_env_var(env, key, "##NO_VALUE##");
		free(key);
	}
	return (0);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	char	*new_str;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len > n)
		len = n;
	new_str = (char *)malloc(len + 1);
	if (!new_str)
		return (NULL);
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
		return (print_sorted_env(shell->env_list, 1), 0);
	ret = 0;
	i = 0;
	while (args[++i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
			key = ft_strndup(args[i], eq - args[i]);
		else
			key = ft_strdup(args[i]);
		if (!is_valid_env_name(key))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else if (process_export_arg(&shell->env_list, args[i]))
			ret = 1;
		free(key);
	}
	return (ret);
}
