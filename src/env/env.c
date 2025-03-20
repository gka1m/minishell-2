/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkaim <gkaim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:53:42 by theophane         #+#    #+#             */
/*   Updated: 2025/03/07 14:16:12 by gkaim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

/* reference from header*/
// typedef struct s_minishell
// {
//     char **env;          // Array of environment variables (similar to environ)
//     int last_exit_code;  // Stores the exit code of the last executed command ($?)
//     char cwd[PATH_MAX];  // Current working directory
//     int interactive;     // Boolean: 1 for interactive mode, 0 for non-interactive
// } t_minishell;

// to init minishell structure
// could also be in the wrong file, maybe minishell.c?
/* t_minishell *init_minishell(char **envp)
{
    t_minishell *minishell = malloc(sizeof(t_minishell));
    if (!minishell)
    {
        perror("malloc error for minishell");
        exit(EXIT_FAILURE);
    }

    // Copy the environment variables
    int env_count = 0;
    while (envp[env_count])
        env_count++;
    minishell->env = malloc((env_count + 1) * sizeof(char *));
    if (!minishell->env)
    {
        perror("malloc error for env");
        free(minishell);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < env_count; i++)
    {
        minishell->env[i] = ft_strdup(envp[i]);
    }
    minishell->env[env_count] = NULL; // NULL-terminate the array

    // Initialize other fields
    minishell->last_exit_code = 0;
    if (!getcwd(minishell->cwd, PATH_MAX))
    {
        perror("getcwd");
        free(minishell->env);
        free(minishell);
        exit(EXIT_FAILURE);
    }
    minishell->interactive = isatty(STDIN_FILENO); // 1 if shell is interactive

    return minishell;
} */

t_minishell *init_minishell(char **envp)
{
    t_minishell *minishell;

    minishell = malloc(sizeof(t_minishell));
    if (!minishell)
    {
        perror("malloc error for minishell");
        exit(EXIT_FAILURE);
    }
    init_fields(minishell, envp);
    minishell->interactive = isatty(STDIN_FILENO);
    return (minishell);
}

void    init_fields(t_minishell *minishell, char **envp)
{
    int env_count;
    int i;

    i = -1;
    env_count = 0;
    while (envp[env_count])
        env_count++;
    minishell->env = malloc((env_count + 1) * sizeof(char *));
    if (!minishell->env)
    {
        perror("malloc error for env");
        free(minishell);
        exit(EXIT_FAILURE);
    }
    while (++i < env_count)
        minishell->env[i] = ft_strdup(envp[i]);
    minishell->env[env_count] = NULL;
    minishell->last_exit_code = 0;
    if (!getcwd(minishell->cwd, PATH_MAX))
    {
        perror("getcwd");
        free(minishell->env);
        free(minishell);
        exit(EXIT_FAILURE);
    }
}

// function to free memory after exit
// should be a utils somewhere
void    free_minishell(t_minishell *minishell)
{
    int i;

    i = 0;
    // Free the environment variables
    while (minishell->env[i])
    {
        free(minishell->env[i]);
        i++;
    }
    free(minishell->env);

    // Free the minishell struct itself
    free(minishell);
}

/* mains for testing the minishell environment*/
// int main(int argc, char **argv, char **envp) {
//     // Initialize the minishell state
//     t_minishell *minishell = init_minishell(envp);

//     // Main shell loop
//     while (1) {
//         char input[1024];

//         if (minishell->interactive) {
//             printf("%s> ", minishell->cwd); // Display prompt
//         }

//         if (!fgets(input, sizeof(input), stdin)) { // Handle ctrl-D (EOF)
//             if (minishell->interactive) printf("exit\n");
//             break;
//         }

//         // Process the input (parse, execute, etc.)
//         // For now, simulate an exit command
//         if (strcmp(input, "exit\n") == 0) break;

//         // Example: Update last_exit_code (to mimic a successful command)
//         minishell->last_exit_code = 0; 
//     }

//     // Cleanup
//     free_minishell(minishell);
//     return 0;
// }
