/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkaim <gkaim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:13:34 by zchan             #+#    #+#             */
/*   Updated: 2025/03/07 16:06:23 by gkaim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

int execute_builtin(const char *cmd, char **args, t_minishell *minishell)
{
    if (ft_strncmp(cmd, "echo", 4) == 0)
        return (builtin_echo(args));
    else if (ft_strncmp(cmd, "cd", 2) == 0)
        return (builtin_cd(args, minishell));
    else if (ft_strncmp(cmd, "pwd", 3) == 0)
        return (builtin_pwd());
    else if (ft_strncmp(cmd, "export", 6) == 0)
        return (builtin_export(args, minishell));
    else if (ft_strncmp(cmd, "unset", 5) == 0)
        return (builtin_unset(args, minishell));
    else if (ft_strncmp(cmd, "env", 3) == 0)
        return (builtin_env(minishell));
    else if (ft_strncmp(cmd, "exit", 4) == 0)
        return (builtin_exit(args));
    return (-1); // Not a built-in
}

// echo
// Prints arguments
// Supports the -n option to suppress the newline at the end
int builtin_echo(char **args)
{
    int i;
    int newline;
    
    i = 1;
    newline = 1; // By default, append a newline
    if (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
    {
        newline = 0; // Suppress newline
        i++;
    }

    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" "); // Add space between arguments
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}

// cd
// Changes the current directory to the path specified in args[1]
// Only supports relative or absolute paths.
int builtin_cd(char **args, t_minishell *minishell)
{
    if (!args[1])
    {
        printf("cd: missing argument\n");
        return (-1);
    }
    if (chdir(args[1]) == -1)
    {
        perror("cd");
        return (-1);
    }
    getcwd(minishell->cwd, PATH_MAX); // Update current working directory
    return (0);
}

// pwd
// Prints the current working directory
int builtin_pwd()
{
    char cwd[PATH_MAX];
    
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("pwd");
        return (-1);
    }
    printf("%s\n", cwd);
    return (0);
}

// export
// Adds or updates environment variables
// Expects arguments in the format VAR=value
int builtin_export(char **args, t_minishell *minishell)
{
    int i;
    
    i = 1;
    if (!args[i])
    {
        print_env_vars(minishell->env); // Print all environment variables
        return (0);
    }

    while (args[i])
    {
        if (!is_valid_env_format(args[i]))
        {
            printf("export: invalid format: %s\n", args[i]);
            return (-1);
        }
        add_or_update_env_var(args[i], minishell);
        i++;
    }
    return (0);
}

/*ANYTHING BELOW HERE IS REFERENCE AND RESEARCH MATERIAL
// Stub out built-ins for now and handle cd as an example.
int execute_builtin(const char *cmd, char **args, t_minishell *minishell)
{
    if (strcmp(cmd, "cd") == 0)
    {
        if (chdir(args[1]) == -1)
        {
            perror("cd");
            return -1;
        }
        getcwd(minishell->cwd, PATH_MAX);
        return 0;
    }
    // Add other built-ins
    return -1; // Not a built-in
}

*/