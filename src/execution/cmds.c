/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:45 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/07 14:21:10 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_builtin(t_minishell *shell, char **args, int fd_out)
{
	if (!args[0])
		return (0);
	if (ft_strncmp(args[0], "cd", 2) == 0)
		return (b_cd(shell, &shell->env_list, args));
	if (ft_strncmp(args[0], "echo", 4) == 0)
		return (b_echo(args));
	if (ft_strncmp(args[0], "env", 3) == 0)
		return (b_env(shell->env_list));
	if (ft_strncmp(args[0], "exit", 4) == 0)
		return (b_exit(shell, args));
	if (ft_strncmp(args[0], "export", 6) == 0)
		return (bi_export(shell, args, fd_out));
	if (ft_strncmp(args[0], "unset", 5) == 0)
		return (b_unset(shell->env_list, args));
	if (ft_strncmp(args[0], "pwd", 3) == 0)
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
		if (setup_redirections(node, shell) == -1)
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
		handle_parent_process(pid, shell);
}

char *find_command_path(char *cmd, t_env *env_list)
{
    // Handle absolute/relative paths
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) == -1) {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd, STDERR_FILENO);
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            return NULL;
        }
        if (access(cmd, X_OK) == -1) {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd, STDERR_FILENO);
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
            return NULL;
        }
        return ft_strdup(cmd);
    }
    // Get PATH from environment
    t_env *path_var = find_env_var(env_list, "PATH");
    if (!path_var || !path_var->value) {
        ft_putstr_fd("minishell: PATH not set\n", STDERR_FILENO);
        return NULL;
    }
    // Split PATH into directories using ft_split
    char **dirs = ft_split(path_var->value, ':');
    if (!dirs)
        return NULL;
    // Search through each directory
    char *full_path = NULL;
    for (int i = 0; dirs[i]; i++)
    {
        full_path = join_str(dirs[i], "/", cmd);
        if (!full_path)
            continue;
            
        if (access(full_path, F_OK) == 0) {
            if (access(full_path, X_OK) == 0) {
                free_split(dirs);
                return full_path;
            }
            free(full_path);
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd, STDERR_FILENO);
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
            break;
        }
        free(full_path);
    }
    free_split(dirs);
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd, STDERR_FILENO);
    ft_putstr_fd(": command not found\n", STDERR_FILENO);
    return NULL;
}

void execute_command(t_ast *node, t_minishell *shell)
{
    if (!node || !node->args || !node->args[0])
        return;
    // Check if command is a builtin
    if (is_builtin(node->args[0])) {
        shell->last_exit_code = execute_builtin(shell, node->args, STDOUT_FILENO);
        return;
    }
    // Handle external commands
    execute_external(node, shell);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strncmp(cmd, "cd", 2) == 0 || ft_strncmp(cmd, "echo", 4) == 0
		|| ft_strncmp(cmd, "env", 3) == 0 || ft_strncmp(cmd, "exit", 4) == 0
		|| ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "export", 6) == 0
		|| ft_strncmp(cmd, "unset", 5) == 0);
}

void	handle_parent_process(pid_t pid, t_minishell *shell)
{
	int status;
	pid_t waited_pid;

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
			write(STDERR_FILENO, "Quit: 3\n", 8);
	}
}
