/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:58:57 by zchan             #+#    #+#             */
/*   Updated: 2025/03/22 16:25:31 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

int	change_to_home(char **args)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
	{
		printf("cd: HOME not set\n");
		return (-1);
	}
	return (chdir(home));
}

// Print the directory being switched to (Bash behavior)
// Then chdir
int	change_to_oldpwd(char **args)
{
	char	*oldpwd;

	oldpwd = getenv("OLDPWD");
	if (!oldpwd)
	{
		printf("cd: OLDPWD not set\n");
		return (-1);
	}
	printf("%s\n", oldpwd);
	return (chdir(oldpwd));
}

// Expand '~' to $HOME, strcat args[1] +1
int	change_to_expanded_home(char **args)
{
	char	*home;
	char	full_path[PATH_MAX];

	home = getenv("HOME");
	if (!home)
	{
		printf("cd: HOME not set\n");
		return (-1);
	}
	ft_strcpy(full_path, home);
	ft_strcat(full_path, args[1] + 1);
	return (chdir(full_path));
}

// Function to simulate `getcwd` in custom environment
// Simulate updating the environment variables (e.g., OLDPWD and PWD)
int	update_oldpwd_and_pwd(t_minishell *minishell, t_env *env_list,
		char oldpwd[PATH_MAX])
{
	char	*home;

	home = get_env_value(env_list, "HOME");
	if (!home)
	{
		printf("cd: HOME not set\n");
		return (-1);
	}
	if (set_env_value(&env_list, "OLDPWD", oldpwd) == -1)
		return (-1);
	if (set_env_value(&env_list, "PWD", minishell->cwd) == -1)
		return (-1);
	return (0);
}

/*
// Standard directory change
int	change_to_directory(char **args)
{
	return (chdir(args[1]));
}
*/
