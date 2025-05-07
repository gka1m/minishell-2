/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:35:22 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/07 14:39:29 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// int	is_valid_exit_arg(char *arg)
// {
// 	int	i;

// 	i = 0;
// 	if (arg[i] == '-' || arg[i] == '+')
// 		i++;
// 	while (arg[i])
// 	{
// 		if (!ft_isdigit(arg[i]))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

int	is_valid_exit_arg(char *arg)
{
	int	i;
	int	digits;

	i = 0;
	digits = 0;
	// Optional sign
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	// Check each digit
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		digits++;
		i++;
	}
	// Empty string or just a sign isn't valid
	if (digits == 0)
		return (0);
	// Check if number exceeds LONG_MIN/LONG_MAX
	if (digits > 20) // Max digits for 64-bit long
		return (0);
	return (1);
}

static int handle_overflow(const char *str)
{
    long long num;
    int sign;
    int i;

    num = 0;
    sign = 1;
    i = 0;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        if (num > (LLONG_MAX - (str[i] - '0')) / 10)
        {
            if (sign == 1)
                return (255);
            else
                return (0);
        }
        num = num * 10 + (str[i] - '0');
        i++;
    }
    return (num * sign);
}

void	cleanup_and_exit(t_minishell *shell, int exit_code)
{
	// Add any necessary cleanup here
	// free_env(shell->env_list);
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->ast)
		free_ast(shell->ast);
	free_minishell(shell);
	// Free other resources as needed
	exit(exit_code);
	// shell->last_exit_code = exit_code;
}

// int	b_exit(t_minishell *shell, char **args)
// {
// 	int exit_code;

// 	if (!args[1])
// 		cleanup_and_exit(shell, shell->last_exit_code);
// 	if (!is_valid_exit_arg(args[1]))
// 	{
// 		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
// 		ft_putstr_fd(args[1], STDERR_FILENO);
// 		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
// 		cleanup_and_exit(shell, 2);
// 	}
// 	if (args[2])
// 	{
// 		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
// 		shell->last_exit_code = 1;
// 		return (1); // Don't exit if too many arguments
// 	}
// 	exit_code = ft_atoi(args[1]);
// 	cleanup_and_exit(shell, exit_code);
// 	return (0); // This line won't normally be reached
// }

int	b_exit(t_minishell *shell, char **args)
{
	int exit_code;
	int arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count == 1)
		cleanup_and_exit(shell, shell->last_exit_code);
	if (args[1] && !is_valid_exit_arg(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		return (2);
	}

	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1); // Don't exit
	}
	// Handle number conversion with overflow
	exit_code = handle_overflow(args[1]);
	// Convert to unsigned 8-bit (bash behavior)
	if (exit_code < 0)
		exit_code = 256 - (-exit_code % 256);
	else if (exit_code > 255)
		exit_code %= 256;
	cleanup_and_exit(shell, exit_code);
	return (exit_code); // Unreachable
}
