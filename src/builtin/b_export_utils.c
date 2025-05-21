/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:23:07 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/21 12:26:50 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_empty_assignment(const char *arg)
{
	const char	*eq;

	eq = ft_strchr(arg, '=');
	return (eq && (*(eq + 1) == '\0' || (*(eq + 1) == '"' && *(eq
					+ 2) == '"')));
}

int	invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}
