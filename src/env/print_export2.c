/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:41:18 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/19 11:44:03 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Prints all variables in sorted order */
void	print_sorted_env(t_env *env, int fd_out)
{
	char	**keys;
	t_env	*var;
	int		i;

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
	char	*tmp;

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
		return ;
	size = 0;
	while (tab[size])
		size++;
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
