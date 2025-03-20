/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkaim <gkaim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 12:52:21 by zchan             #+#    #+#             */
/*   Updated: 2025/03/07 14:32:24 by gkaim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

// print env
void    print_env_vars(char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
}

// check env var is valid
int is_valid_env_format(const char *arg)
{
    return (ft_strchr(arg, '=') != NULL); // Ensure the format contains '='
}

// find env var
int find_env_var(const char *var, char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], var, ft_strlen(var)) == 0 &&
            env[i][ft_strlen(var)] == '=')
        {
            return (i); // Return index of the variable
        }
        i++;
    }
    return (-1); // Not found
}

// add or update env var
/* void    add_or_update_env_var(const char *new_var, t_minishell *minishell)
{
    int     i;
    int     var_index;
    int     count;
    char    **new_env;
    
    var_index = find_env_var(new_var, minishell->env);
    if (var_index != -1)
    {
        // Update existing variable
        free(minishell->env[var_index]);
        minishell->env[var_index] = ft_strdup(new_var);
    }
    // Add new variable
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
} */

void remove_env_var(const char *var, t_minishell *minishell)
{
    int var_index;
    int i;
    
    var_index = find_env_var(var, minishell->env);
    if (var_index != -1)
    {
        // Free the variable and shift the remaining variables
        free(minishell->env[var_index]);
        i = var_index;
        while (minishell->env[i])
        {
            minishell->env[i] = minishell->env[i + 1];
            i++;
        }
    }
}
