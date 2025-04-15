/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:02:18 by zchan             #+#    #+#             */
/*   Updated: 2025/04/15 15:26:14 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
// #include "../../libft/libft.h"

// Redirection types
#define INPUT_REDIRECT 1
#define OUTPUT_REDIRECT 2
#define APPEND_REDIRECT 3
#define HERE_DOC 4

// Main funciton
int handle_redirection(t_ast *ast)
{
    if (!ast || !ast->file)
    {
        fprintf(stderr, "Invalid redirection node\n");
        return -1;
    }

    if (ast->type == AST_REDIR_IN)
        return handle_input_redirection(ast);
    else if (ast->type == AST_REDIR_OUT || ast->type == AST_APPEND)
        return handle_output_redirection(ast);
    else if (ast->type == AST_HEREDOC)
        return handle_here_document(ast);
    else
    {
        fprintf(stderr, "Unsupported redirection type\n");
        return -1;
    }
}

// Handle Input Redirection (<)
int handle_input_redirection(t_ast *ast)
{
    int fd = open(ast->file, O_RDONLY);
    if (fd < 0)
    {
        perror("open input file");
        return -1;
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

// Handle Output Redirection (> and >>)
int handle_output_redirection(t_ast *ast)
{
    int fd;
    int flags;

    // Base flags for write-only mode and file creation
    flags = O_WRONLY | O_CREAT;
    if (ast->type == AST_APPEND)
    {
        flags |= O_APPEND; // Append to the file for '>>'
    }
    else
    {
        flags |= O_TRUNC;  // Overwrite the file for '>'
    }
    fd = open(ast->file, flags, 0644);
    if (fd < 0)
    {
        perror("open output file");
        return -1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}



/*Version 1
int	handle_redirection(t_node *node, t_minishell *minishell)
{
	int		fd;
	int		flags;
		char buffer[1024];
		char temp_filename[] = "/tmp/minishell_heredoc_XXXXXX";
	ssize_t	bytes_read;

	// Check for valid node
	if (!node || !node->filename)
	{
		fprintf(stderr, "Invalid redirection node\n");
		return (-1);
	}
	if (node->redir_type == INPUT_REDIRECT)
	{
		// Input redirection: '< filename'
		fd = open(node->filename, O_RDONLY);
		if (fd < 0)
		{
			perror("open input file");
			return (-1);
		}
		dup2(fd, STDIN_FILENO); // Redirect stdin
		close(fd);
	}
	else if (node->redir_type == OUTPUT_REDIRECT
		|| node->redir_type == APPEND_REDIRECT)
	{
		// Output redirection: '> filename' or '>> filename'
		flags = O_WRONLY | O_CREAT;
		flags |= (node->redir_type == APPEND_REDIRECT) ? O_APPEND : O_TRUNC;
		fd = open(node->filename, flags, 0644);
		if (fd < 0)
		{
			perror("open output file");
			return (-1);
		}
		dup2(fd, STDOUT_FILENO); // Redirect stdout
		close(fd);
	}
	else if (node->redir_type == HERE_DOC)
	{
		int fd = mkstemp(temp_filename); // Create a temporary file
		if (fd < 0)
		{
			perror("mkstemp");
			return (-1);
		}
		// Collect user input for the here-document
		printf("heredoc> ");
		while (1)
		{
			bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
			if (bytes_read < 0)
			{
				perror("read");
				close(fd);
				unlink(temp_filename);
				return (-1);
			}
			if (bytes_read == 0)
			{
				break ; // End of input (EOF)
			}
			buffer[bytes_read] = '\0'; // Null-terminate the buffer
			// Check if the delimiter is encountered
			if (strncmp(buffer, node->filename, strlen(node->filename)) == 0
				&& buffer[strlen(node->filename)] == '\n')
			{
				break ; // Stop when the delimiter is entered
			}
			// Write the input to the temporary file
			if (write(fd, buffer, bytes_read) < 0)
			{
				perror("write");
				close(fd);
				unlink(temp_filename);
				return (-1);
			}
			printf("heredoc> ");
		}
		close(fd); // Close the temporary file after writing
		// Reopen the temporary file for reading
		fd = open(temp_filename, O_RDONLY);
		if (fd < 0)
		{
			perror("reopen temporary file");
			unlink(temp_filename);
			return (-1);
		}
		dup2(fd, STDIN_FILENO); // Redirect stdin to the temporary file
		close(fd);              // Close the file descriptor after redirection
		unlink(temp_filename);  // Delete the temporary file
	}
	else
	{
		fprintf(stderr, "Unsupported redirection type\n");
		return (-1);
	}
	return (0);
}

// Simplified version
// A separate function to manage redirections (<, >, >>, <<).
int	handle_redirection(t_node *node, t_minishell *minishell)
{
	int	fd;
	int	flags;

	if (node->redir_type == INPUT_REDIRECT)
	{
		fd = open(node->filename, O_RDONLY);
		if (fd < 0) {
			perror("open input file");
			return (-1);
		}
		dup2(fd, STDIN_FILENO); // Redirect stdin
		close(fd);
	}
	else if (node->redir_type == OUTPUT_REDIRECT)
	{
		flags = O_WRONLY | O_CREAT | (node->append ? O_APPEND : O_TRUNC);
		fd = open(node->filename, flags, 0644);
		if (fd < 0)
		{
			perror("open output file");
			return (-1);
		}
		dup2(fd, STDOUT_FILENO); // Redirect stdout
		close(fd);
	}
	return (0);
}
*/

/* ANYTHING BELOW HERE IS REFERENCE AND RESEARCH MATERIAL

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// EXAMPLE
// Here’s how to implement redirection (e.g., command > file or command < file):


void	execute_command_with_redirection(char *cmd, char **args, char *infile,
		char *outfile, int append) {
	int in_fd, out_fd;

	// Handle input redirection
	if (infile != NULL) {
		in_fd = open(infile, O_RDONLY);
		if (in_fd < 0) {
			perror("open input file");
			exit(EXIT_FAILURE);
		}
		dup2(in_fd, STDIN_FILENO); // Redirect stdin
		close(in_fd);
	}

	// Handle output redirection
	if (outfile != NULL) {
		int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
		out_fd = open(outfile, flags, 0644);
		if (out_fd < 0) {
			perror("open output file");
			exit(EXIT_FAILURE);
		}
		dup2(out_fd, STDOUT_FILENO); // Redirect stdout
		close(out_fd);
	}

	// Execute the command
	if (execvp(cmd, args) == -1) {
		perror("execvp");
		exit(EXIT_FAILURE);
	}
}

// Main function to test redirection
int	main(void) {
	char *args[] = {"echo", "Hello, World!", NULL};
	execute_command_with_redirection("echo", args, NULL, "output.txt", 0);
		// Write to output.txt
	return (0);
}
*/