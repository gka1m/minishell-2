/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:58:25 by gkaim             #+#    #+#             */
/*   Updated: 2025/03/22 14:56:49 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

// unset
// Removes environment variables by name
// Check if variable exists, if not (-1) print message
// Remove the variable if it does
int	builtin_unset(char **args, t_minishell *minishell)
{
	int	i;

	i = 1;
	if (!args[i])
	{
		printf("unset: missing argument\n");
		return (-1);
	}
	while (args[i])
	{
		if (find_env_var(args[i], minishell->env) == -1)
			printf("unset: %s: no such variable\n", args[i]);
		else
			remove_env_var(args[i], minishell);
		i++;
	}
	return (0);
}

// env
// Check if environment is empty, print messasge
// Prints all environment variables
int	builtin_env(t_minishell *minishell)
{
	int	i;

	if (!minishell->env || !minishell->env[0])
	{
		printf("env: environment is empty\n");
		return (1);
	}
	i = 0;
	while (minishell->env[i])
	{
		printf("%s\n", minishell->env[i]);
		i++;
	}
	return (0);
}

// exit
// Exits the shell with an optional exit code (args[1])
// Use 255 as the error exit code if not numeric (as per bash)
int	builtin_exit(char **args)
{
	int	exit_code;

	exit_code = 0;
	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			printf("exit: %s: numeric argument required\n", args[1]);
			exit(255);
		}
		exit_code = ft_atoi(args[1]);
	}
	exit(exit_code);
	return (0);
}

// Allow negative or positive signs
// Hence ft_isdigit not enough
static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
