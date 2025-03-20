/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkaim <gkaim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:58:25 by gkaim             #+#    #+#             */
/*   Updated: 2025/03/07 16:07:18 by gkaim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

// unset
// Removes environment variables by name
int builtin_unset(char **args, t_minishell *minishell)
{
    int i;
    
    i = 1;
    if (!args[i])
    {
        printf("unset: missing argument\n");
        return (-1);
    }

    while (args[i])
    {
        remove_env_var(args[i], minishell);
        i++;
    }
    return (0);
}

// env
// Prints all environment variables
int builtin_env(t_minishell *minishell)
{
    int i;

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
int builtin_exit(char **args)
{
    int exit_code;
    
    exit_code = 0;
    if (args[1])
        exit_code = ft_atoi(args[1]);
    exit(exit_code);
    return (0); // This line will never be reached
}
