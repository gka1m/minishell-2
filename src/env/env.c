/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:53:42 by theophane         #+#    #+#             */
/*   Updated: 2025/05/29 09:36:13 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

/*
// (reference from header)
// Array of environment variables (similar to environ)
// Stores the exit code of the last executed command ($?)
// Current working directory
// Boolean: 1 for interactive mode, 0 for non-interactive
typedef struct s_minishell
{
	char	**env;
	int		last_exit_code;
	char	cwd[PATH_MAX];
	int		interactive;
}			t_minishell;
*/

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
	{
		perror("malloc error for minishell");
		free_split(envp);
		return (NULL);
	}
	init_fields(minishell, envp);
	minishell->interactive = isatty(STDIN_FILENO);
	minishell->stdio_backup[0] = -1;
	minishell->stdio_backup[1] = -1;
	return (minishell);
}

void	init_fields(t_minishell *minishell, char **envp)
{
	minishell->env_list = init_env(envp);
	if (!minishell->env_list)
	{
		perror("environment initialization failed");
		free(minishell);
		exit(EXIT_FAILURE);
	}
	update_shlvl(minishell->env_list);
	minishell->last_exit_code = 0;
	if (!getcwd(minishell->cwd, PATH_MAX))
	{
		perror("getcwd");
		free_env(minishell->env_list);
		free(minishell);
		exit(EXIT_FAILURE);
	}
}

// function to free memory after exit
// should be a utils somewhere
// Free the environment variables then free struct itself
void	free_minishell(t_minishell *minishell)
{
	if (minishell->env_list)
	{
		free_env(minishell->env_list);
	}
	if (minishell->stdio_backup[0] != -1)
		close(minishell->stdio_backup[0]);
	if (minishell->stdio_backup[1] != -1)
		close(minishell->stdio_backup[1]);
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
//             break ;
//         }

//         // Process the input (parse, execute, etc.)
//         // For now, simulate an exit command
//         if (ft_strcmp(input, "exit\n") == 0) break ;

//         // Example: Update last_exit_code (to mimic a successful command)
//         minishell->last_exit_code = 0;
//     }

//     // Cleanup
//     free_minishell(minishell);
//     return (0);
// }

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

	return (minishell);
} */