/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:35:47 by theophane         #+#    #+#             */
/*   Updated: 2025/03/10 13:33:15 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

volatile sig_atomic_t   g_signal_flag = 0;

// Function to execute external or builtin commands
int execute_command(t_ast *ast, t_minishell *minishell)
{
    pid_t pid;
    int status;
    int fd_in = STDIN_FILENO;
    int fd_out = STDOUT_FILENO;

    // Check if a signal was received
    if (g_signal_flag)
    {
        fprintf(stderr, "Command interrupted by signal\n");
        g_signal_flag = 0; // Reset the signal flag
        return -1; // Return an error
    }

    // Validate input
    if (!ast || !ast->args || !ast->args[0])
    {
        // fprintf(stderr, "Invalid command node\n");
        return -1;
    }

    // Handle heredoc (<<)
    if (ast->type == AST_HEREDOC)
    {
        if (handle_here_document(ast) == -1)
        {
            return -1; // Heredoc setup failed
        }
    }

    // Handle other redirections (>, <, >>)
    if (ast->type == AST_REDIR_IN || ast->type == AST_HEREDOC)
    {
        fd_in = open(ast->file, O_RDONLY);
        if (fd_in < 0)
        {
            perror("open");
            return -1;
        }
    }
    else if (ast->type == AST_REDIR_OUT || ast->type == AST_APPEND)
    {
        int flags = O_WRONLY | O_CREAT;
        if (ast->type == AST_APPEND)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;
        fd_out = open(ast->file, flags, 0644);
        if (fd_out < 0)
        {
            perror("open");
            return -1;
        }
    }

    // Check if it's a builtin command
    if (is_builtin(ast->args[0]))
        return execute_builtin(ast->args[0], ast->args, minishell);

    // External command execution
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        // Child process
        if (fd_in != STDIN_FILENO)
        {
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if (fd_out != STDOUT_FILENO)
        {
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        char *full_path = resolve_command_path(ast->args[0], minishell->env);
        if (!full_path)
        {
            fprintf(stderr, "%s: command not found\n", ast->args[0]);
            return -1;
        }
        if (execve(full_path, ast->args, minishell->env) == -1)
        {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for the child
    waitpid(pid, &status, 0);

    // Check if a signal was received while waiting
    if (g_signal_flag)
    {
        fprintf(stderr, "Command interrupted by signal\n");
        g_signal_flag = 0; // Reset the signal flag
        return -1; // Return an error
    }

    // Close file descriptors if they were opened
    if (fd_in != STDIN_FILENO)
        close(fd_in);
    if (fd_out != STDOUT_FILENO)
        close(fd_out);

    // Check exit status
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return -1;
}

// function to check if command is builtin command
int is_builtin(const char *cmd)
{
    if (!cmd)
        return 0;

    return (!strcmp(cmd, "echo") ||
            !strcmp(cmd, "cd") ||
            !strcmp(cmd, "pwd") ||
            !strcmp(cmd, "export") ||
            !strcmp(cmd, "unset") ||
            !strcmp(cmd, "env") ||
            !strcmp(cmd, "exit"));
}

char *resolve_command_path(const char *cmd, char **env)
{
    if (!cmd || !env)
        return NULL;

    // Check if the command already contains a path (e.g., /bin/ls)
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0) // Check if the file is executable
            return strdup(cmd); // Return a copy of the full path
        return NULL; // Command not found or not executable
    }

    // Get the PATH environment variable
    char *path = NULL;
    int i = 0;
    while (env[i])
    {
        if (strncmp(env[i], "PATH=", 5) == 0)
        {
            path = env[i] + 5; // Skip "PATH="
            break;
        }
        i++;
    }

    if (!path)
        return NULL; // PATH not found

    // Split PATH into individual directories using ft_split
    char **dirs = ft_split(path, ':');
    if (!dirs)
        return NULL; // Memory allocation failed

    // Search each directory for the command
    char *full_path = NULL;
    int j = 0;
    while (dirs[j])
    {
        // Construct the full path (e.g., /bin/ls)
        full_path = malloc(strlen(dirs[j]) + strlen(cmd) + 2); // +2 for '/' and '\0'
        if (!full_path)
        {
            // Free allocated memory for dirs
            int k = 0;
            while (dirs[k])
                free(dirs[k++]);
            free(dirs);
            return NULL; // Memory allocation failed
        }
        sprintf(full_path, "%s/%s", dirs[j], cmd);

        // Check if the file exists and is executable
        if (access(full_path, X_OK) == 0)
        {
            // Free allocated memory for dirs
            int k = 0;
            while (dirs[k])
                free(dirs[k++]);
            free(dirs);
            return full_path; // Return the full path
        }

        free(full_path); // Free the constructed path if not executable
        j++;
    }

    // Free allocated memory for dirs
    int k = 0;
    while (dirs[k])
        free(dirs[k++]);
    free(dirs);

    return NULL; // Command not found in PATH
}

/*ANYTHING BELOW HERE IS REFERENCE AND RESEARCH MATERIAL

// A function to execute built-in commands (cd, echo, etc.).
int execute_builtin(char *command, char **args, t_minishell *minishell)
{
    if (strcmp(command, "cd") == 0)
    {
        if (args[1] == NULL)
        {
            fprintf(stderr, "cd: missing argument\n");
            return -1;
        }
        if (chdir(args[1]) != 0)
        {
            perror("chdir");
            return -1;
        }
        return 0;
    }
    // Add other built-ins (echo, exit, etc.)
    return -1; // Command not a built-in
}

*/