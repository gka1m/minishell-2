/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:45 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/08 14:44:49 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_builtin(t_minishell *shell, char **args, int fd_out)
{
	if (!args[0])
		return (0);
	if (strcmp(args[0], "cd") == 0)
		return (b_cd(shell, &shell->env_list, args));
	if (strcmp(args[0], "echo") == 0)
		return (b_echo(args));
	if (strcmp(args[0], "env") == 0)
		return (b_env(shell->env_list));
	if (strcmp(args[0], "exit") == 0)
		return (b_exit(shell, args));
	if (strcmp(args[0], "export") == 0)
		return (bi_export(shell, args, fd_out));
	if (strcmp(args[0], "unset") == 0)
		return (b_unset(shell->env_list, args));
	if (strcmp(args[0], "pwd") == 0)
		return (b_pwd());
	return (0);
}

// void	execute_external(t_ast *node, t_minishell *shell)
// {
// 	pid_t	pid;
// 	char	**env_array;
// 	char	*full_path;

// 	if (!node || !node->args || !node->args[0])
// 		return ;
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("minishell: fork");
// 		shell->last_exit_code = 1;
// 		return ;
// 	}
// 	if (pid == 0)
// 	{ // Child process
// 		sig_reset(true);
// 		if (setup_redirections(node, shell) == -1)
// 			exit(1);
// 		env_array = convert_env_to_array(shell->env_list);
// 		if (!env_array)
// 			exit(1);
// 		full_path = find_command_path(node->args[0], shell); // Pass shell now
// 		if (!full_path)
// 		{
// 			free_split(env_array);
// 			exit(127);
// 		}
// 		execve(full_path, node->args, env_array);
// 		// If execve fails
// 		perror("minishell: execve");
// 		free(full_path);
// 		free_split(env_array);
// 		exit(126);
// 	}
// 	else // Parent process
// 		handle_parent_process(pid, shell);
// }

int	execute_external(t_ast *node, t_minishell *shell)
{
	char	**env_array;
	char	*full_path;
	int		status;

	env_array = NULL;
	full_path = NULL;
	status = 0;
	if (!node || !node->args || !node->args[0])
	{
		ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
		return (127);
	}
	sig_reset(true);
	env_array = convert_env_to_array(shell->env_list);
	if (!env_array)
	{
		return (1);
	}
	full_path = find_command_path(node->args[0], shell);
	if (!full_path)
	{
		free_split(env_array);
		return (127);
	}
	execve(full_path, node->args, env_array);
	// If we get here, execve failed
	perror("minishell: execve");
	free(full_path);
	free_split(env_array);
	return (126);
}

char	*find_command_path(char *cmd, t_minishell *shell)
// Add shell parameter for error reporting
{
	t_env *path_node;
	char **dirs;
	char *full_path;

	if (!cmd || !shell || !shell->env_list)
		return (NULL);
	// Handle absolute/relative paths
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
		return (NULL);
	}
	// Get PATH from environment
	path_node = find_env_var(shell->env_list, "PATH");
	if (!path_node || !path_node->value)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": PATH not set\n", STDERR_FILENO);
		return (NULL);
	}
	// Split PATH into directories
	dirs = ft_split(path_node->value, ':');
	if (!dirs)
		return (NULL);
	// Search each directory
	full_path = NULL;
	for (int i = 0; dirs[i]; i++)
	{
		full_path = join_str(dirs[i], "/", cmd);
		if (!full_path)
			continue ;
		if (access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) == 0)
			{
				free_split(dirs);
				return (full_path);
			}
			free(full_path);
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			break ;
		}
		free(full_path);
	}
	free_split(dirs);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	return (free_ast(shell->ast), free_tokens(shell->tokens), NULL);
}

// void	execute_command(t_ast *node, t_minishell *shell)
// {
// 	pid_t	pid;

// 	while (node && node->type != AST_CMD)
// 		node = node->left;
// 	if (!node || !node->args || !node->args[0])
// 		return ;
// 	sig_reset(false);

// 	if (is_builtin(node->args[0]))
// 	{
// 		// Redirection requires fork for builtins
// 		if (setup_redirections(node, shell) == -1)
// 			return ;
// 		if (node->type == AST_APPEND || node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT || node->type == AST_HEREDOC)
// 		{
// 			pid = fork();
// 			if (pid == 0)
// 			{
// 				sig_reset(true);
// 				exit(execute_builtin(shell, node->args, STDOUT_FILENO));
// 			}
// 			else
// 			{
// 				handle_parent_process(pid, shell);
// 				restore_standard_fds(shell);
// 			}
// 		}
// 		else
// 		{
// 			shell->last_exit_code = execute_builtin(shell, node->args, STDOUT_FILENO);
// 		}
// 	}
// 	else
// 	{
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			sig_reset(true);
// 			if (setup_redirections(node, shell) == -1)
// 				cleanup_and_exit(shell, 1);
// 			execute_external(node, shell);
// 			exit(shell->last_exit_code);
// 		}
// 		else
// 		{
// 			handle_parent_process(pid, shell);
// 			restore_standard_fds(shell);
// 		}
// 	}
// 	sig_interactive();
// }

void	execute_command(t_ast *node, t_minishell *shell)
{
	pid_t	pid;
	int		redir_applied;
	int		status;

	redir_applied = 0;
	// Find the command node (AST_CMD)
	while (node && node->type != AST_CMD)
		node = node->left;
	if (!node || !node->args || !node->args[0])
		return ;
	sig_reset(false);
	// Handle builtins in parent process
	if (is_builtin(node->args[0]))
	{
		if (setup_redirections(node, shell) == -1)
		{
			shell->last_exit_code = 1;
			return ;
		}
		redir_applied = 1;
		shell->last_exit_code = execute_builtin(shell, node->args,
				STDOUT_FILENO);
		if (redir_applied)
			restore_standard_fds(shell);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			// Child process
			sig_reset(true);
			if (setup_redirections(node, shell) == -1)
				exit(1);
			status = execute_external(node, shell);
			free_minishell(shell);
			exit(status);
		}
		else if (pid > 0)
		{
			// Parent process
			handle_parent_process(pid, shell);
			restore_standard_fds(shell);
		}
		else
		{
			perror("minishell: fork");
			shell->last_exit_code = 1;
		}
	}
	sig_interactive();
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "echo") == 0 || strcmp(cmd,
			"env") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "pwd") == 0
		|| strcmp(cmd, "export") == 0 || strcmp(cmd, "unset") == 0);
}

void	handle_parent_process(pid_t pid, t_minishell *shell)
{
	int		status;
	pid_t	waited_pid;

	// Wait for the child process to complete
	waited_pid = waitpid(pid, &status, 0);
	if (waited_pid == -1)
	{
		perror("minishell: waitpid");
		shell->last_exit_code = 1;
		return ;
	}
	// Set exit status based on how the child terminated
	if (WIFEXITED(status))
		shell->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		// Terminated by signal
		shell->last_exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit: Core dumped\n", 18);
	}
}
