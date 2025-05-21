/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:38:28 by kagoh             #+#    #+#             */
/*   Updated: 2025/05/21 14:38:54 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_itoa_into(char *buf, int n)
{
	int		len;
	int		num;
	char	tmp[16];
	int		i;
	int		j;

	len = 0;
	num = n;
	if (n == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return ;
	}
	i = 0;
	while (num > 0)
	{
		tmp[i++] = '0' + (num % 10);
		num /= 10;
	}
	j = -1;
	while (++j < i)
		buf[len++] = tmp[i - j - 1];
	buf[len] = '\0';
}

void	ft_strcpy(char *dst, const char *src)
{
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
}

void	ft_strcat(char *dst, const char *src)
{
	while (*dst)
		dst++;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
}
