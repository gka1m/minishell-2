/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:45 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/03 16:21:48 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_builtin(t_minishell *shell, char **args, int fd_out)
{
	if (!args[0])
		return (0);
	if (ft_strcmp(args[0], "cd") == 0)
		return (b_cd(shell, &shell->env_list, args));
	if (ft_strcmp(args[0], "echo") == 0)
		return (b_echo(args));
	if (ft_strcmp(args[0], "env") == 0)
		return (b_env(shell->env_list));
	if (ft_strcmp(args[0], "exit") == 0)
		return (b_exit(shell, args));
	if (ft_strcmp(args[0], "export") == 0)
		return (bi_export(shell, args, fd_out));
	if (ft_strcmp(args[0], "unset") == 0)
		return (b_unset(shell->env_list, args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (b_pwd());
	return (0);
}

void	execute_external(t_ast *node, t_minishell *shell)
{
	pid_t	pid;
	char	**env_array;
	char	*full_path;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		shell->last_exit_code = 1;
		return ;
	}
	if (pid == 0) // Child process
	{
		// Set default signal handlers
		sig_reset(true);
		// Handle redirections
		if (setup_redirections(node) == -1)
			exit(1);
		// Convert environment
		env_array = convert_env_to_array(shell->env_list);
		if (!env_array)
			exit(1);
		// Find and execute command
		full_path = find_command_path(node->args[0], shell->env_list);
		if (!full_path)
		{
			error_command_not_found(node->args[0]);
			free_split(env_array);
			exit(127);
		}
		execve(full_path, node->args, env_array);
		// Handle execve errors
		handle_exec_error(node->args[0], full_path, env_array);
		exit((errno == EACCES) ? 126 : 1);
	}
	else // Parent process
	{
		handle_parent_process(pid, shell);
	}
}

char	*find_command_path(char *cmd, t_env *env_list)
{
	char	*path;
	char	**dirs;
	char	*full_path;
	int		i;

	// Handle absolute/relative paths
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
		{
			return (ft_strdup(cmd));
		}
		return (NULL);
	}
	// Get PATH from environment
	path = find_env_var(env_list, "PATH");
	if (!path)
		return (NULL);
	// Split PATH into directories
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	// Search through each directory
	i = 0;
	while (dirs[i])
	{
		full_path = join_str(dirs[i], "/", cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_split(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(dirs);
	return (NULL);
}

void	execute_command(t_ast *node, t_minishell *shell)
{
	char	**env_array;
	int		builtin_status;

	if (!node || !node->args)
		return ;
	// Check if command is a builtin
	builtin_status = execute_builtin(shell, node->args, STDOUT_FILENO);
	if (builtin_status != 0 || is_builtin(node->args[0]))
	{
		shell->last_exit_code = builtin_status;
		return ;
	}
	// Handle external commands
	execute_external(node, shell);
}
