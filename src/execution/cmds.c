/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:45 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/22 14:11:41 by kagoh            ###   ########.fr       */
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

	env_array = NULL;
	full_path = NULL;
	if (!node || !node->args || !node->args[0])
		return (ft_putstr_fd("minishell: command not found\n", 2), 127);
	sig_reset(true);
	env_array = convert_env_to_array(shell->env_list);
	if (!env_array)
		return (1);
	full_path = find_command_path(node->args[0], shell);
	if (!full_path)
		return (free_split(env_array), 127);
	close(shell->stdio_backup[1]);
	close(shell->stdio_backup[0]);
	execve(full_path, node->args, env_array);
	perror("minishell: execve");
	free(full_path);
	free_split(env_array);
	return (126);
}

char	*check_direct_path(char *cmd)
{
	if (access(cmd, F_OK) != 0)
		return (no_file(cmd), NULL);
	if (access(cmd, X_OK) != 0)
		return (cmd_not_found(cmd), NULL);
	return (ft_strdup(cmd));
}

char	*search_in_path(char **dirs, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (dirs[i])
	{
		full_path = join_str(dirs[i], "/", cmd);
		if (!full_path)
		{
			i++;
			continue ;
		}
		if (access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) == 0)
				return (free_split(dirs), full_path);
			free(full_path);
			permission_denied(cmd);
			break ;
		}
		free(full_path);
		i++;
	}
	return (free_split(dirs), cmd_not_found(cmd), NULL);
}

char	*find_command_path(char *cmd, t_minishell *shell)
{
	t_env	*path_node;
	char	**dirs;

	// char *full_path;
	// int	i;
	if (!cmd || !shell || !shell->env_list)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		// if (access(cmd, F_OK) == 0)
		// {
		// 	if (access(cmd, X_OK) == 0)
		// 		return (ft_strdup(cmd));
		// 	cmd_not_found(cmd);
		// }
		// else
		// 	no_file(cmd);
		// return (NULL);
		return (check_direct_path(cmd));
	}
	path_node = find_env_var(shell->env_list, "PATH");
	if (!path_node || !path_node->value)
		return (path_not_set(cmd), NULL);
	dirs = ft_split(path_node->value, ':');
	if (!dirs)
		return (NULL);
	// full_path = NULL;
	// i = 0;
	// while(dirs[i])
	// {
	// 	full_path = join_str(dirs[i], "/", cmd);
	// 	if (!full_path)
	// 		continue ;
	// 	if (access(full_path, F_OK) == 0)
	// 	{
	// 		if (access(full_path, X_OK) == 0)
	// 			return (free_split(dirs), full_path);
	// 		free(full_path);
	// 		permission_denied(cmd);
	// 		break ;
	// 	}
	// 	free(full_path);
	// 	i++;
	// }
	// return (cmd_not_found(cmd), free_split(dirs), NULL);
	return (search_in_path(dirs, cmd));
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
// 		if (node->type == AST_APPEND || node->type == AST_REDIR_IN
			// || node->type == AST_REDIR_OUT || node->type == AST_HEREDOC)
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
// 			shell->last_exit_code = execute_builtin(shell, node->args,
					// STDOUT_FILENO);
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
