/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:19:30 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/12 14:32:46 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void    print_args(char **args, int nflag)
{
    int i;

    i = 1;
    if (args[i] && strcmp(args[i], "-n") == 0)
        i++;
    while(args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putchar_fd(' ', 1);
        i++;
    }
    if (!nflag)
        ft_putchar_fd('\n', 1);
}

int b_echo(char **args)
{
    int nflag;

    nflag = 0;
    if (args[1] && strcmp(args[1], "-n") == 0)
        nflag = 1;
    print_args(args, nflag);
    return (0);
}
