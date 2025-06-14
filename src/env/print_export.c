/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:57:33 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/29 09:36:13 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// int	is_valid_env_name(const char *str)
// {
// 	if (!str || !*str || ft_isdigit(*str))
// 		return (0);
// 	while (*str && *str != '=')
// 	{
// 		if (!ft_isalnum(*str) && *str != '_')
// 			return (0);
// 		str++;
// 	}
// 	return (1);
// }

int	is_valid_env_name(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (str[0] == '=')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		if (i == 0 && ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/* Prints a single variable in declare -x format */
// void	print_export_var(t_env *var, int fd_out)
// {
// 	ft_putstr_fd("declare -x ", fd_out);
// 	ft_putstr_fd(var->key, fd_out);
// 	if (var->value)
// 	{
// 		ft_putstr_fd("=\"", fd_out);
// 		ft_putstr_fd(var->value, fd_out);
// 		ft_putstr_fd("\"", fd_out);
// 	}
// 	ft_putstr_fd("\n", fd_out);
// }

void	print_export_var(t_env *var, int fd_out)
{
	ft_putstr_fd("declare -x ", fd_out);
	ft_putstr_fd(var->key, fd_out);
	if (var->value && ft_strcmp(var->value, "##NO_VALUE##") == 0)
	{
		ft_putstr_fd("\n", fd_out);
		return ;
	}
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
