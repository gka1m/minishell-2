/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:35:22 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/14 15:40:37 by kagoh            ###   ########.fr       */
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
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		digits++;
		i++;
	}
	if (digits == 0)
		return (0);
	if (digits > 20)
		return (0);
	return (1);
}

int	handle_overflow(const char *str)
{
	long long	num;
	int			sign;
	int			i;

	num = 0;
	sign = 1;
	i = -1;
	if (str[++i] == '-' || str[++i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
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
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->ast)
		free_ast(shell->ast);
	restore_standard_fds(shell);
	free_minishell(shell);
	exit(exit_code);
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
	int	exit_code;
	int	arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count == 1)
		cleanup_and_exit(shell, shell->last_exit_code);
	if (args[1] && !is_valid_exit_arg(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
	if (arg_count > 2)
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2), 1);
	exit_code = handle_overflow(args[1]);
	if (exit_code < 0)
		exit_code = 256 - (-exit_code % 256);
	else if (exit_code > 255)
		exit_code %= 256;
	cleanup_and_exit(shell, exit_code);
	return (exit_code);
}
