/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:49:27 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/01 16:06:41 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Processes a single export argument (VAR or VAR=value) */
int	process_export_arg(t_env **env, char *arg)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	int		key_len;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		// Manual key extraction without strndup
		key_len = equal_sign - arg;
		key = malloc(key_len + 1);
		if (!key)
			return (1);
		ft_strlcpy(key, arg, key_len + 1); // or manual copy
		value = ft_strdup(equal_sign + 1);
		if (!value)
			return (free(key), 1);
		update_env_var(*env, key, value);
		free(key);
		free(value);
	}
	else if (!find_env_var(*env, arg))
		add_env_var(env, arg, NULL);
	return (0);
}

/* Main export builtin function */
int	bi_export(t_minishell *shell, char **args, int fd_out)
{
	int	i;
	int	ret;

	(void)fd_out; // For future redirection support
	if (!args[1])
	{
		print_sorted_env(shell->env_list, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	ret = EXIT_SUCCESS;
	i = 1;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = EXIT_FAILURE;
		}
		else if (process_export_arg(&shell->env_list, args[i]))
			ret = EXIT_FAILURE;
		i++;
	}
	return (ret);
}
