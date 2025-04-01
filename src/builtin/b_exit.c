/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:35:22 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/01 14:35:43 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_exit_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	cleanup_and_exit(t_minishell *shell, int exit_code)
{
	// Add any necessary cleanup here
	free_env(shell->env_list);
	// Free other resources as needed
	exit(exit_code);
}

int	b_exit(t_minishell *shell, char **args)
{
	int exit_code;

	if (!args[1])
		cleanup_and_exit(shell, shell->last_exit_code);
	if (!is_valid_exit_arg(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		cleanup_and_exit(shell, 2);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		shell->last_exit_code = 1;
		return (1); // Don't exit if too many arguments
	}
	exit_code = ft_atoi(args[1]);
	cleanup_and_exit(shell, exit_code);
	return (0); // This line won't normally be reached
}
