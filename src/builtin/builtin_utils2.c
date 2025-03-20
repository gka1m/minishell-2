/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkaim <gkaim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:30:34 by gkaim             #+#    #+#             */
/*   Updated: 2025/03/07 14:32:13 by gkaim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

void    update_env_var(const char *new_var, int var_index, t_minishell *minishell)
{
    free(minishell->env[var_index]);
    minishell->env[var_index] = ft_strdup(new_var);
}

void    add_env_var(const char *new_var, t_minishell *minishell)
{
    int     i;
    int     count;
    char    **new_env;

    count = 0;
    while (minishell->env[count])
        count++;
    new_env = malloc((count + 2) * sizeof(char *));
    if (!new_env)
    {
        perror("malloc");
        return ;
    }
    i = -1;
    while (++i < count)
        new_env[i] = minishell->env[i];
    new_env[count] = ft_strdup(new_var);
    new_env[count + 1] = NULL;
    free(minishell->env);
    minishell->env = new_env;
}

void    add_or_update_env_var(const char *new_var, t_minishell *minishell)
{
    int var_index;

    var_index = find_env_var(new_var, minishell->env);
    if (var_index != -1)
        update_env_var(new_var, var_index, minishell);
    else
        add_env_var(new_var, minishell);
}
