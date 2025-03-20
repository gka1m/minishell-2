/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:00:24 by zchan             #+#    #+#             */
/*   Updated: 2025/03/06 13:10:00 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*VERSION 2*/

// Helper function for handling pipe redirections
int redirect_pipe_end(int pipe_fd, int std_fd, int close_fd)
{
    if (dup2(pipe_fd, std_fd) == -1)
    {
        perror("dup2");
        close(pipe_fd);
        close(close_fd);
        return -1;
    }
    close(pipe_fd);
    close(close_fd);
    return 0;
}

// Helper function for creating a child process with pipe redirection
pid_t create_pipe_child_process(t_pipechild config)
{
    pid_t   child_pid;
    
    child_pid = fork();
    if (child_pid < 0)
    {
        perror("fork");
        close(config.read_fd);
        close(config.write_fd);
        return -1;
    }

    if (child_pid == 0)
    {
        // Child process
        if (redirect_pipe_end(config.write_fd, config.std_fd, config.read_fd) == -1)
        {
            exit(EXIT_FAILURE);
        }
        
        // Recursive execution of the appropriate side of the pipe
        exit(execution_logic(config.ast, config.minishell));
    }
    return child_pid;
}

/* Simple version
// Function to handle redirection in a command
int execute_redirection(t_node *ast, t_minishell *minishell)
{
    // Validate input
    if (!ast)
    {
        fprintf(stderr, "Invalid redirection node\n"); // or return some error
        return -1;
    }

    // Call existing redirection handler
    if (handle_redirection(ast, minishell) == -1)
    {
        fprintf(stderr, "Redirection failed\n"); // or return some error
        return -1;
    }
    
    // Recursively execute the child command
    return execution_logic(ast->child, minishell);
}
*/

// Enhanced version to save/restore OG stdin stdout
// See research redir.txt
int execute_redirection(t_ast *ast, t_minishell *minishell)
{
    int stdin_backup;
    int stdout_backup;
    int result;

    // Validate input
    if (!ast)
    {
        fprintf(stderr, "Invalid redirection node\n");
        return -1;
    }

    // Save original stdin and stdout
    stdin_backup = dup(STDIN_FILENO);
    stdout_backup = dup(STDOUT_FILENO);
    if (stdin_backup == -1 || stdout_backup == -1)
    {
        perror("dup");
        return -1;
    }

    // Call existing redirection handler
    if (handle_redirection(ast) == -1)
    {
        fprintf(stderr, "Redirection failed\n");
        // Restore original file descriptors on failure
        dup2(stdin_backup, STDIN_FILENO);
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdin_backup);
        close(stdout_backup);
        return -1;
    }

    // After redirection is set up, execute the child node of the redirection AST (usually the command to be executed).
    // Execute the child command
    result = execute_command(ast->left, minishell); // Use ast->left for the child command

    // Restore original stdin and stdout
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);

    // Return the result of executing the child command to propagate the exit status.
    return result;
}

// Using above defined struct, else too many variables for norm
// config the child process
t_pipechild create_pipe_child_config(t_ast *ast, t_minishell *minishell,
                                     int pipe_fds[2], int std_fd)
{
    t_pipechild config;

    config = (t_pipechild){
        .ast = ast,
        .minishell = minishell,
        .read_fd = pipe_fds[0],
        .write_fd = pipe_fds[1],
        .std_fd = std_fd
    };
    return config;
}

// Existing create_pipe_child_process and other functions remain the same
// pid_t create_pipe_child_process(t_pipechild config)
// {
//     pid_t child_pid;

//     child_pid = fork();
//     if (child_pid < 0)
//     {
//         perror("fork");
//         close(config.read_fd);
//         close(config.write_fd);
//         return -1;
//     }

//     if (child_pid == 0)
//     {
//         // Child process
//         if (redirect_pipe_end(config.write_fd, config.std_fd, config.read_fd) == -1)
//         {
//             exit(EXIT_FAILURE);
//         }

//         // Recursive execution of the appropriate side of the pipe
//         exit(execute_command(config.ast, config.minishell)); // Replace execution_logic with execute_command
//     }

//     return child_pid;
// }


// Helper function to close pipe file descriptors
void close_pipe_fds(int pipe_fd[2])
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
}

// Helper function to check pipe creation
int setup_pipe(int pipe_fd[2])
{
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return -1;
    }
    return 0;
}

// Helper function to handle left child process creation
int handle_left_child(t_ast *ast, t_minishell *minishell, int pipe_fd[2], pid_t *left_pid)
{
    t_pipechild left_config;

    // Create configuration for left child process
    left_config = create_pipe_child_config(ast->left, minishell, pipe_fd, STDOUT_FILENO);

    // Create child process for left side of pipe
    *left_pid = create_pipe_child_process(left_config);
    if (*left_pid < 0)
    {
        close_pipe_fds(pipe_fd);
        return -1;
    }
    return 0;
}

// Helper function to handle right child process creation
int handle_right_child(t_ast *ast, t_minishell *minishell, int pipe_fd[2], pid_t *right_pid)
{
    t_pipechild right_config;

    // Create configuration for right child process
    right_config = create_pipe_child_config(ast->right, minishell, pipe_fd, STDIN_FILENO);

    // Create child process for right side of pipe
    *right_pid = create_pipe_child_process(right_config);
    if (*right_pid < 0)
    {
        close_pipe_fds(pipe_fd);
        // kill(left_pid, SIGTERM); // check needed or how to implement this
        // waitpid(left_pid, NULL, 0); // let the parent process handle cleanup if failure
        return -1;
    }
    return 0;
}

// Helper function to check child process exit statuses
int check_child_exit_status(int left_status, int right_status)
{
    if (WIFEXITED(left_status) && WIFEXITED(right_status))
    {
        if (WEXITSTATUS(left_status) == 0 && WEXITSTATUS(right_status) == 0)
            return 0;
    }
    return -1;
}

// Modified execute_pipe function
int execute_pipe(t_ast *ast, t_minishell *minishell)
{
    int pipe_fd[2];
    pid_t left_pid;
    pid_t right_pid;
    int left_status;
    int right_status;

    // Validate input
    if (!ast || !ast->left || !ast->right)
    {
        fprintf(stderr, "Invalid pipe configuration\n");
        return -1;
    }

    // Setup pipe
    if (setup_pipe(pipe_fd) == -1)
        return -1;

    // Handle left child process
    if (handle_left_child(ast, minishell, pipe_fd, &left_pid) == -1)
    {
        close_pipe_fds(pipe_fd);
        return -1;
    }

    // Handle right child process
    if (handle_right_child(ast, minishell, pipe_fd, &right_pid) == -1)
    {
        kill(left_pid, SIGTERM);    // Kill the left child process if it has already been forked
        waitpid(left_pid, NULL, 0); // Cleanup left process
        close_pipe_fds(pipe_fd);
        return -1;
    }

    // Close pipe file descriptors in parent process
    close_pipe_fds(pipe_fd);

    // Wait for both child processes
    waitpid(left_pid, &left_status, 0);
    waitpid(right_pid, &right_status, 0);

    // Update minishell's last exit code
    minishell->last_exit_code = check_child_exit_status(left_status, right_status);

    // Return status based on child process exits
    return check_child_exit_status(left_status, right_status);
}


// // Main execution logic function
// int execution_logic(t_node *ast, t_minishell *minishell)
// {
//     if (ast == NULL)
//         return;
//     if (ast->type == PIPE)
//         execute_pipe(ast, minishell);
//     else if (ast->type == REDIRECTION)
//         handle_redirection(ast, minishell);
//     else if (ast->type == COMMAND)
//         execute_command(ast, minishell);
//     else
//     {
//         fprintf(stderr, "Unknown node type\n");
//         return -1;
//     }
// }

/* REFERENCE: SWITCH CODE
int execution_logic(t_node *ast, t_minishell *minishell) {
    // Check if the node is NULL (base case for recursion)
    if (!ast) return -1;

    switch (ast->type) {
        case PIPE:
            return execute_pipe(ast, minishell);
        
        case REDIRECTION:
            return execute_redirection(ast, minishell);
        
        case COMMAND:
            return execute_command(ast, minishell);
        
        default:
            fprintf(stderr, "Invalid AST node type\n");
            return -1;
    }
}
*/

/* VERSION 1
// Function to handle pipe execution
int execute_pipe(t_node *ast, t_minishell *minishell) {
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return -1;
    }

    // Fork for left side of pipe
    pid_t left_pid = fork();
    if (left_pid < 0) {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return -1;
    }

    if (left_pid == 0) {
        // Left child process
        close(pipe_fd[0]);
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            close(pipe_fd[1]);
            exit(EXIT_FAILURE);
        }
        close(pipe_fd[1]);
        
        // Recursive call to execute left side
        exit(execution_logic(ast->left, minishell));
    }

    // Fork for right side of pipe
    pid_t right_pid = fork();
    if (right_pid < 0) {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        waitpid(left_pid, NULL, 0);
        return -1;
    }

    if (right_pid == 0) {
        // Right child process
        close(pipe_fd[1]);
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            close(pipe_fd[0]);
            exit(EXIT_FAILURE);
        }
        close(pipe_fd[0]);
        
        // Recursive call to execute right side
        exit(execution_logic(ast->right, minishell));
    }

    // Parent process
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    
    // Wait for both child processes
    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
    
    return 0;
}

// Function to handle redirections
int execute_redirection(t_node *ast, t_minishell *minishell) {
    // Call existing redirection handler
    if (handle_redirection(ast, minishell) == -1) {
        fprintf(stderr, "Redirection failed\n");
        return -1;
    }
    
    // Recursively execute the child command
    return execution_logic(ast->child, minishell);
}

// Function to execute external or builtin commands
int execute_command(t_node *ast, t_minishell *minishell) {
    // Check if it's a builtin command
    if (is_builtin(ast->command)) {
        return execute_builtin(ast->command, ast->args, minishell);
    }

    // External command execution
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Child process
        if (execve(ast->command, ast->args, minishell->env) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for the child
    waitpid(pid, NULL, 0);
    return 0;
}

// Main execution logic function (refactored)
int execution_logic(t_node *ast, t_minishell *minishell) {
    // Check if the node is NULL (base case for recursion)
    if (!ast) return -1;

    switch (ast->type) {
        case PIPE:
            return execute_pipe(ast, minishell);
        
        case REDIRECTION:
            return execute_redirection(ast, minishell);
        
        case COMMAND:
            return execute_command(ast, minishell);
        
        default:
            fprintf(stderr, "Invalid AST node type\n");
            return -1;
    }
}
*/

/* ANYTHING BELOW HERE IS REFERENCE AND RESEARCH MATERIAL

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// EXAMPLE
// Scenario: ls -l | grep txt | wc -l
// This involves three commands:

// ls -l generates output.
// grep txt filters the output for lines containing txt.
// wc -l counts the filtered lines.


// Function to execute a single command
void execute_command(char *cmd, char **args) {
    if (execvp(cmd, args) == -1) {
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

// Function to handle pipes
void execute_pipe(char *commands[][3], int num_commands) {
    int pipe_fds[2]; // Pipe file descriptors
    int prev_fd = -1; // FD to read from previous pipe
    pid_t pid;

    for (int i = 0; i < num_commands; ++i) {
        if (i < num_commands - 1) { // Not the last command
            if (pipe(pipe_fds) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid == 0) { // Child process
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO); // Read input from previous pipe
                close(prev_fd);
            }
            if (i < num_commands - 1) {
                dup2(pipe_fds[1], STDOUT_FILENO); // Write output to next pipe
                close(pipe_fds[1]);
            }
            if (i < num_commands - 1) close(pipe_fds[0]); // Close unused read end

            // Execute command
            execute_command(commands[i][0], commands[i]);
        } else if (pid > 0) { // Parent process
            if (prev_fd != -1) close(prev_fd); // Close old read end
            if (i < num_commands - 1) {
                close(pipe_fds[1]); // Close write end
                prev_fd = pipe_fds[0]; // Save read end for next command
            }
            waitpid(pid, NULL, 0); // Wait for child to finish
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
}
*/
/*
// function to traverse and exec L and R nodes of pipe
t_node  *traverse_pipe(t_node *asy, t_minishell *minishell)
{
    int pipefd[2];
    int pid_left;
    int pid_right;

    if(handle_pipe(pipefd, minishell) == 1)
        return (NULL);
    handle_fork(&pid_left, minishell);
    if(!pid_left)
    {
        // redir the pipe L node;
        // free everything and exit;
    }
    handle_fork(&pid_left, minishell);
    // clean heredoc left node
    if(!pid_right)
    {
        // redir the pipe R node;
        // free everything and exit;
    }
    // redit pipe PARENT
    waitpid(pid_left, &minishell->exit_status, 0);
    waitpid(pid_right, &minishell->exit_status, 0);
    // exit status of minishell;
    return(ast);
}


// Main function to test pipe handling
int main() {
    char *commands[][3] = {
        {"ls", "-l", NULL},
        {"grep", "txt", NULL},
        {"wc", "-l", NULL}
    };

    int num_commands = 3;
    execute_pipe(commands, num_commands);
    return 0;
}
*/

/* THIS IS FROM MEDIUM
What we are basically given in the execution part is an AST, and the way we
 will execute it is by traversing it recursively, here is a pseudo-code that
 traverses the arithmetic AST built earlier in the parsing phase.

// pseudo traversal
int execute(t_node node)
{
 if (node.type == PIPE)
   return (execute_pipe(node.left, node.right));
 else
   return (execute_simple_command(node.value))
}

// pseudo execute pipe
int execute_pipe()
{
   pipe(pipe_fds);
   left_pid = fork();
   if (pid == 0) // inside the left child
   {
      // do stuff
      close(pipe_fds[0]); //Accordingly
      close(pipe_fds[1]); //Accordingly
   }
   else // back to the parent
   {
      //do the same for the right child
      // Make sure to close the pipe_fds in the parent too
   }
}

That’s about the core of the execution part, beside some work on redirections,
 where we basically open a file and then redirect (using dup2) the standard
 input or the standard output to that file, and in case of the here-doc we 
 redirect the standard input to a pipe.
*/
