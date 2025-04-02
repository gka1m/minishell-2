/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:57:33 by kagoh             #+#    #+#             */
/*   Updated: 2025/04/02 12:47:32 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_env_name(const char *str)
{
	if (!str || !*str || ft_isdigit(*str))
		return (0);
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

/* Prints a single variable in declare -x format */
void	print_export_var(t_env *var, int fd_out)
{
	ft_putstr_fd("declare -x ", fd_out);
	ft_putstr_fd(var->key, fd_out);
	if (var->value)
	{
		ft_putstr_fd("=\"", fd_out);
		ft_putstr_fd(var->value, fd_out);
		ft_putstr_fd("\"", fd_out);
	}
	ft_putstr_fd("\n", fd_out);
}

/* Creates a NULL-terminated array of environment keys */
char	**env_to_keys_array(t_env *env)
{
	char	**keys;
	int		count;
	t_env	*tmp;
	int		i;

	count = 0;
	tmp = env;
	while (tmp && ++count)
		tmp = tmp->next;
	keys = malloc(sizeof(char *) * (count + 1));
	if (!keys)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		keys[i++] = tmp->key;
		tmp = tmp->next;
	}
	keys[i] = NULL;
	return (keys);
}

/* Prints all variables in sorted order */
void	print_sorted_env(t_env *env, int fd_out)
{
	char **keys;
	t_env *var;
	int i;

	keys = env_to_keys_array(env);
	if (!keys)
		return ;

	sort_keys(keys);
	i = -1;
	while (keys[++i])
	{
		var = find_env_var(env, keys[i]);
		print_export_var(var, fd_out);
	}
	free(keys);
}

/* Swaps two string pointers */
void	swap_strings(char **a, char **b)
{
	char *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/* Simple bubble sort for string array */
void	sort_keys(char **tab)
{
	int	i;
	int	j;
	int	size;

	if (!tab || !*tab)
		return;
	// Calculate array size
	size = 0;
	while (tab[size])
		size++;
	// Bubble sort
	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (strcmp(tab[j], tab[j + 1]) > 0)
				swap_strings(&tab[j], &tab[j + 1]);
			j++;
		}
		i++;
	}
}
