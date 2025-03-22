/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:13:34 by zchan             #+#    #+#             */
/*   Updated: 2025/03/22 16:06:53 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

// Return -1 if not a built-in
int	execute_builtin(const char *cmd, char **args, t_minishell *minishell)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (builtin_echo(args));
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (builtin_cd(args, minishell));
	else if (ft_strncmp(cmd, "pwd", 3) == 0)    
		return (builtin_pwd());
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (builtin_export(args, minishell));
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (builtin_unset(args, minishell));
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (builtin_env(minishell));
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (builtin_exit(args));
	return (-1);
}

// echo
// Prints arguments
// Supports the -n option to suppress the newline at the end
// By default, append a newline (newline = 1). Suppress newline if "-n"
// Add space between arguments "printf(" ")"
int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

// cd
// Changes the current directory to the path specified in args[1]
// Save the current working directory before changing it
// Update `$OLDPWD` and `$PWD`
int builtin_cd(char **args, t_minishell *minishell)
{
    char oldpwd[PATH_MAX];

    if (!getcwd(oldpwd, sizeof(oldpwd)))
    {
        perror("getcwd");
        return (-1);
    }
    if (!args[1] || strcmp(args[1], "--") == 0)
        return change_to_home(args);
    else if (ft_strcmp(args[1], "-") == 0)
        return change_to_oldpwd(args);
    else if (args[1][0] == '~')
        return change_to_expanded_home(args);
    else
        return change_to_directory(args); // return chdir(args[1]);
    return update_oldpwd_and_pwd(minishell, oldpwd);
}

// pwd
// Prints the current working directory
int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (-1);
	}
	printf("%s\n", cwd);
	return (0);
}

// export
// Adds or updates environment variables
// Expects arguments in the format VAR=value
// if !args[i], print all environment variables
// (first letter of key needs to be _ or alpha, nothing else)
// (key exists (eg "a"), you export with no value, it don't change)
int	builtin_export(char **args, t_minishell *minishell)
{
	int	i;

	i = 1;
	if (!args[i])
	{
		print_env_vars(minishell->env);
		return (0);
	}
	while (args[i])
	{
		if (!is_valid_env_format(args[i]))
		{
			printf("export: invalid format: %s\n", args[i]);
			return (-1);
		}
		add_or_update_env_var(args[i], minishell);
		i++;
	}
	return (0);
}


/* OLD SIMPLE VERSION
// cd
// Changes the current directory to the path specified in args[1]
// Update current working directory "getcwd(minishell->cwd, PATH_MAX);"
int	builtin_cd(char **args, t_minishell *minishell)
{
	if (!args[1])
	{
		printf("cd: missing argument\n");
		return (-1);
	}
	if (chdir(args[1]) == -1)
	{
		perror("cd");
		return (-1);
	}
	getcwd(minishell->cwd, PATH_MAX);
	return (0);
}

NEW LONG VERSION
// cd ..    up one dir "chdir(args[1])"
// cd .     stays in current dir "chdir(args[1])"
// cd       $HOME
// cd ~/Desktop $HOME
// cd -     $OLDPWD (previous directory)
// cd --    behave like cd ie $HOME
int builtin_cd(char **args, t_minishell *minishell)
{
    char *dest;
    char oldpwd[PATH_MAX];

    // Save the current working directory before changing it
    if (!getcwd(oldpwd, sizeof(oldpwd))) {
        perror("getcwd");
        return (-1);
    }

    // Case: `cd` or `cd --` → Go to `$HOME`
    if (!args[1] || strcmp(args[1], "--") == 0) {
        dest = getenv("HOME");
        if (!dest) {
            printf("cd: HOME not set\n");
            return (-1);
        }
    }
    // Case: `cd -` → Switch to `$OLDPWD`
    else if (strcmp(args[1], "-") == 0) {
        dest = getenv("OLDPWD");
        if (!dest) {
            printf("cd: OLDPWD not set\n");
            return (-1);
        }
        printf("%s\n", dest);  // Print the directory being switched to (Bash behavior)
    }
    // Case: `cd ~/Desktop` → Expand `~` to `$HOME`
    else if (args[1][0] == '~') {
        char *home = getenv("HOME");
        if (!home) {
            printf("cd: HOME not set\n");
            return (-1);
        }
        // Construct full path: `$HOME/Desktop`
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s%s", home, args[1] + 1);
        dest = full_path;
    }
    else {
        dest = args[1];  // Standard directory change
    }

    // Attempt to change directory
    if (chdir(dest) == -1) {
        perror("cd");
        return (-1);
    }

    // Update `$OLDPWD` and `$PWD`
    setenv("OLDPWD", oldpwd, 1);
    getcwd(minishell->cwd, PATH_MAX);
    setenv("PWD", minishell->cwd, 1);

    return (0);
}
*/