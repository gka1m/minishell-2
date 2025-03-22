/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 12:52:21 by zchan             #+#    #+#             */
/*   Updated: 2025/03/22 14:33:16 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"

// print env
void	print_env_vars(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

// check env var is valid
// Ensure the format contains '='
int	is_valid_env_format(const char *arg)
{
	return (ft_strchr(arg, '=') != NULL);
}

// find env var
// Return index of the variable
// Return -1 if not found
int	find_env_var(const char *var, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) == 0
			&& env[i][ft_strlen(var)] == '=')
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

// if -1
// free the variable
// and shift the remaining variables left using while loop
// Ensure the last entry is NULL
void	remove_env_var(const char *var, t_minishell *minishell)
{
	int	var_index;
	int	i;

	var_index = find_env_var(var, minishell->env);
	if (var_index != -1)
	{
		free(minishell->env[var_index]);
		i = var_index;
		while (minishell->env[i])
		{
			minishell->env[i] = minishell->env[i + 1];
			i++;
		}
		minishell->env[i] = NULL;
	}
}

// add or update env var
/* void    add_or_update_env_var(const char *new_var, t_minishell *minishell)
{
	int		i;
	int		var_index;
	int		count;
	char	**new_env;
	int		var_index;
	int		i;

	var_index = find_env_var(new_var, minishell->env);
	if (var_index != -1)
	{
		// Update existing variable
		free(minishell->env[var_index]);
		minishell->env[var_index] = ft_strdup(new_var);
	}
	// Add new variable
	count = 0;
	while (minishell->env[count])
		count++;
	new_env = malloc((count + 2) * sizeof(char *));
	if (!new_env)
	{
		perror("malloc");
		return ;
	}
	i = -1;
	while (++i < count)
		new_env[i] = minishell->env[i];
	new_env[count] = ft_strdup(new_var);
	new_env[count + 1] = NULL;
	free(minishell->env);
	minishell->env = new_env;
} */