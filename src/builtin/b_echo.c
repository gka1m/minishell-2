/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:19:30 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/26 15:49:14 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

void	print_args(char **args, int nflag_start)
{
	int	i;

	i = 1;
	while (args[i] && is_n_flag(args[i]))
		i++;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!nflag_start)
		ft_putchar_fd('\n', 1);
}

int	b_echo(char **args)
{
	int	nflag;

	nflag = 0;
	if (args[1] && is_n_flag(args[1]))
		nflag = 1;
	print_args(args, nflag);
	return (0);
}

