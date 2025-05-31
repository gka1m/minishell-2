/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:59:32 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/31 18:03:08 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	append_char(char **result, char *ch)
{
	char	*temp;

	temp = ft_strjoin(*result, ch);
	free(*result);
	if (temp)
		*result = temp;
}

void	append_str(char **result, char *str)
{
	char	*temp;

	temp = ft_strjoin(*result, str);
	free(*result);
	if (temp)
		*result = temp;
}

void	handle_single_quote(int *i, char **result, int *in_squote)
{
	*in_squote = !(*in_squote);
	append_char(result, "'");
	(*i)++;
}

void	handle_double_quote(int *i, char **result, int *in_dquote)
{
	*in_dquote = !(*in_dquote);
	append_char(result, "\"");
	(*i)++;
}
