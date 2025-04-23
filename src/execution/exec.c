/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:09:44 by zchan             #+#    #+#             */
/*   Updated: 2025/04/23 15:48:44 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* We prob need a folder for all errors as well to dump all error handling there*/

// Main logic for executing
// See below for LONG full version to refactor
// int	execution_logic(t_ast *ast, t_minishell *minishell)
// {
// 	if (ast == NULL)
// 		return (-1);
// 	// Handle commands and their redirections first
// 	if (ast->type == AST_CMD)
// 	{
// 		if (setup_redirections(ast, minishell) == -1)
// 			return (-1);
// 		execute_command(ast, minishell);
// 	}
// 	// Handle pipe nodes (right-recursive structure)
// 	else if (ast->type == AST_PIPE)
// 	{
// 		execute_pipeline(ast, minishell);
// 	}
// 	// Handle standalone redirections
// 	else if (ast->type == AST_REDIR_IN || ast->type == AST_REDIR_OUT
// 		|| ast->type == AST_APPEND || ast->type == AST_HEREDOC)
// 		// Helper macro/function
// 	{
// 		if (setup_redirections(ast, minishell) == -1)
// 			return (-1);
// 		if (ast->left)
// 			return (execution_logic(ast->left, minishell));
// 	}
// 	else
// 	{
// 		ft_putstr_fd("minishell: unknown node type\n", STDERR_FILENO);
// 		return (-1);
// 	}
// 	return (0);
// }

int	execution_logic(t_ast *ast, t_minishell *minishell)
{
	if (!ast)
		return (-1);

	// Apply redirections FIRST (from the root down)
	if (setup_redirections(ast, minishell) == -1)
		return (-1);

	if (ast->type == AST_CMD || ast->type == AST_REDIR_IN || ast->type == AST_REDIR_OUT
		|| ast->type == AST_APPEND || ast->type == AST_HEREDOC)
	{
		execute_command(ast, minishell); // no need to check type again
	}
	else if (ast->type == AST_PIPE)
	{
		execute_pipeline(ast, minishell);
	}
	else
	{
		ft_putstr_fd("minishell: unknown node type\n", STDERR_FILENO);
		return (-1);
	}
	return (0);
}

/* LONG VERSION
// The main recursive function to traverse and handle the AST nodes
// Split logic by PIPE, REDIRECTION, and COMMAND
// Redirection logic is extracted into handle_redirection
// Check for built-ins to avoid unnecessary fork.
// Recursive: Execution logic traverses the AST recursively for PIPE and REDIRECTION.
int	execution_logic(t_node *ast, t_minishell *minishell)
{
		int pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	pid_t	pid;

	// Check if the node is NULL (base case for recursion)
	if (!ast)
		return (-1);
	if (ast->type == PIPE)
	{
		// Set up the pipe
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (-1);
		}
		left_pid = fork();
		if (left_pid == 0)
		{
			// Child process for the left command
			close(pipe_fd[0]); // Close unused read end
			dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to pipe
			close(pipe_fd[1]); // Close write end
			exit(execution_logic(ast->left, minishell)); // Execute left side
		}
		else if (left_pid < 0)
		{
			perror("fork");
			return (-1);
		}
		right_pid = fork();
		if (right_pid == 0)
		{
			// Child process for the right command
			close(pipe_fd[1]); // Close unused write end
			dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to pipe
			close(pipe_fd[0]); // Close read end
			exit(execution_logic(ast->right, minishell)); // Execute right side
		}
		else if (right_pid < 0)
		{
			perror("fork");
			return (-1);
		}
		// Parent process
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		waitpid(left_pid, NULL, 0); // Wait for left child
		waitpid(right_pid, NULL, 0); // Wait for right child
		return (0);
	}
	else if (ast->type == REDIRECTION)
	{
		// Handle redirections and execute the child node
		if (handle_redirection(ast, minishell) == -1)
		{
			fprintf(stderr, "Redirection failed\n");
			return (-1);
		}
		return (execution_logic(ast->child, minishell));
			// Execute the child command
	}
	else if (ast->type == COMMAND)
	{
		// Execute a command (either a built-in or external)
		if (is_builtin(ast->command))
		{
			return (execute_builtin(ast->command, ast->args, minishell));
		}
		else
		{
			pid = fork();
			if (pid == 0)
			{
				// Child process
				if (execve(ast->command, ast->args, minishell->env) == -1)
				{
					perror("execve");
					exit(EXIT_FAILURE);
				}
			}
			else if (pid < 0)
			{
				perror("fork");
				return (-1);
			}
			// Parent process waits for the child
			waitpid(pid, NULL, 0);
			return (0);
		}
	}
	return (-1); // Invalid AST node type
}
*/

/*
// THIS PART IS WHILE WAITING FOR KAGOH AST, FOR OWN CODE TESTING FIRST

t_node	*create_mock_command_node(char *cmd, char **args) {
	t_node *node = malloc(sizeof(t_node));
	node->type = COMMAND;
	node->command = cmd;
	node->args = args;
	return (node);
}

t_node	*create_mock_pipe_node(t_node *left, t_node *right) {
	t_node *node = malloc(sizeof(t_node));
	node->type = PIPE;
	node->left = left;
	node->right = right;
	return (node);
}

// need to re-write the code block above to this
int	execution_logic(t_node *ast, t_minishell *minishell)
{
	if (ast == NULL) return ;
	if (ast->type == PIPE)
		execute_pipe(ast, minishell);
	else if (ast->type == REDIRECTION)
		handle_redirection(ast, minishell);
	else if (ast->type == COMMAND)
		execute_command(ast, minishell);
	else
	{
		fprintf(stderr, "Unknown node type\n");
		return (-1);
	}
}

// Example Usage
char	*args1[] = {"ls", "-l", NULL};
t_node	*cmd1 = create_mock_command_node("ls", args1);

char	*args2[] = {"grep", "test", NULL};
t_node	*cmd2 = create_mock_command_node("grep", args2);

t_node	*pipe_node = create_mock_pipe_node(cmd1, cmd2);
execution_logic(pipe_node, minishell);

*/

/*ANYTHING BELOW HERE IS REFERENCE AND RESEARCH MATERIAL

// PROTOTYPE

//  WIP
void	execute_ast(Node *ast) {
	if (ast == NULL) return ;

	if (ast->type == COMMAND) {
		execute_command(ast->command);
	} else if (ast->type == PIPE) {
		execute_pipe(ast);
	} else if (ast->type == REDIRECTION) {
		handle_redirection(ast);
	} else {
		fprintf(stderr, "Unknown node type\n");
	}
}


Common Sources of Memory Leaks:
Forgetting to free allocated memory (e.g., AST nodes, command history).
Overwriting pointers without freeing the original memory.
Best Practices:
Explicit Ownership:
Decide who is responsible for freeing memory (e.g.,
	parser frees tokens after AST creation).
Freeing AST Nodes:
Use a recursive function to traverse and free all child nodes.
Command History:
Free strings stored in history when they’re no longer needed.
Debugging Tools:
Use tools like valgrind to detect leaks.

//  freeing AST
void	free_ast(Node *node) {
	if (!node) return ;

	switch (node->type) {
		case NODE_COMMAND:
			free(node->data.command.command);
			for (char **arg = node->data.command.args; *arg; ++arg) {
				free(*arg);
			}
			free(node->data.command.args);
			break ;
		case NODE_PIPE:
			free_ast(node->data.pipe.left);
			free_ast(node->data.pipe.right);
			break ;
		case NODE_REDIRECTION:
			free_ast(node->data.redirection.child);
			free(node->data.redirection.filename);
			break ;
	}
	free(node);
}


// Sample AST Structure
typedef enum {
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECTION
}		NodeType;

typedef struct Node {
	NodeType type;
	union {
		struct {
			char *command;
			char **args;
		} command;
		struct {
			struct Node *left;
			struct Node *right;
		} pipe;
		struct {
			struct Node *child;
			char *filename;
			int append; // 0 for >, 1 for >>
		} redirection;
	} data;
}		Node;

// Sample traversal
void	execute_ast(Node *node) {
	if (!node) return ;

	switch (node->type) {
		case NODE_COMMAND:
			execute_command(node->data.command.command,
				node->data.command.args);
			break ;
		case NODE_PIPE:
			execute_pipe_ast(node->data.pipe.left, node->data.pipe.right);
			break ;
		case NODE_REDIRECTION:
			execute_with_redirection(node->data.redirection.child,
				node->data.redirection.filename, node->data.redirection.append);
			break ;
		default:
			fprintf(stderr, "Unknown node type\n");
	}
}

// Not using switch
// consider pipe as first "if"
void	execute_ast(Node *ast) {
	if (ast == NULL)
		return ;
	if (ast->type == COMMAND) {
		execute_command(ast->command);
	} else if (ast->type == PIPE) {
		execute_pipe(ast);
	} else if (ast->type == REDIRECTION) {
		handle_redirection(ast);
	} else {
		fprintf(stderr, "Unknown node type\n");
	}
}


// Example implementation of pipe AST traversal
void	execute_pipe_ast(Node *left, Node *right) {
	int pipe_fds[2];
	if (pipe(pipe_fds) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	if (pid == 0) { // Child
		close(pipe_fds[0]); // Close unused read end
		dup2(pipe_fds[1], STDOUT_FILENO); // Redirect stdout to pipe
		close(pipe_fds[1]);
		execute_ast(left); // Execute left command
	} else if (pid > 0) { // Parent
		close(pipe_fds[1]); // Close unused write end
		dup2(pipe_fds[0], STDIN_FILENO); // Redirect stdin to pipe
		close(pipe_fds[0]);
		execute_ast(right); // Execute right command
		waitpid(pid, NULL, 0); // Wait for child
	} else {
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

// pseudo execute pipe
int	execute(t_node node)
{
 if (node.type == T_PIPE)
   return (execute_pipe(node.left, node.right));
 else
   return (execute_simple_command(node.value))
}

// map out tsechen logic, as reference to do exec
// all in all need 4 big functions to handle (1) PIPE (2) CMD (3) ARG (4) REDIR
int	parsing_logic(t_node *ast)
{
	// assume there is PIPE
	//     build L side
	//         it contains CMD, ARG, REDIR (all these are the tokens in the AST)
	//             CMD is the arr[0]
	//             ARG is the char** to pass into execve
	//             REDIR contains type and descriptor (descriptor is char *)
	//             REDIR has 4 types: < , > , << and >>
	//             Multiple REDIR are possible so it is a linked list
	//             execute each and every one of the linked list to the end
	//     return this value upwards to PIPE
	// if there is a R side (ie assume it has pipe)
	//     assume there is another PIPE
	//         build L side (of the R side of PIPE)
	//             so on so forth
	// else there is no PIPE
	//     return this value upwards to PIPE
}

// int execution_logic(t_node *ast, t_minishell *minishell)
{
	// check the node type
	//     if it's a pipe, then set up the pipes
	//         execute node left in fork 1
	//         execute node right in fork 2
	//         pipe the two.
	//     if it's not a fork
	//         execute the redir first
	//             all of them
	//         then after that, execve
}

// Function to recursively traverse tree to exec nodes
//  When binop found, traverse left
//  If exist status fulfilled, traverse right
//  Leaf node -> exec command
t_node	*traverse_tree(t_node *ast, t_minishell *minishell, t_node *parent_node)
{
	if (!ast || minishell->minishell_err)
		return (ast); // return some sort of error (or exit?),
			depending how bash behaves
	if (node->type == T_PIPE) // if (node-type == T_AND || node->type == T_OR
		|| node->type == T_PIPE)
	{
		if (ast->type == T_PIPE)
			traverse_pipe(ast, minishell);
		else
		{
			traverse_tree(ast->left, minishell, ast);
			if ((!minishell->exit_status && ast->type == T_AND) \
				|| (minishell->exit_status && ast->type == T_OR))
				traverse_tree(ast->right, minishell, ast);
			else if (ast->right && !minishell->exit_status && \
				ast->type == T_OR)
				clean_heredoc(ast->right, minishell);
		}
	}
	else
		exec_command(ast, minishell, parent_node);
	return (ast);
}


// Function to create heredoc nodes and start traversal
t_node	*ft_exec(t_minishell *minishell)
{
	if (minishell->heredoc_count >= 1)
		ft_heredoc(minishell->heredoc_list, minishell);
	if (minishell->ast && !minishell->heredoc_sigint
		&& !minishell->minishell_err)
		traverse_tree(minishell->ast, minishell, minishell->ast);
	return (minishell->ast);
}

*/
