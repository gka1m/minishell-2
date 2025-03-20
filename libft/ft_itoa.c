/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:27:40 by zchan             #+#    #+#             */
/*   Updated: 2024/06/03 15:36:49 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_numlen(int num)
{
	unsigned int	len;

	len = 0;
	if (num < 0)
	{
		num *= -1;
		len++;
	}
	if (num == 0)
		len++;
	while (num != 0)
	{
		num /= 10;
		len++;
	}
	return (len);
}

static void	ft_nbr_printer(char *num, int n, size_t len)
{
	unsigned int	i;

	if (n < 0)
	{
		num[0] = '-';
		i = -n;
	}
	else
		i = n;
	num[len] = '\0';
	while (i != 0)
	{
		num[--len] = (i % 10) + '0';
		i = i / 10;
	}
}

char	*ft_itoa(int n)
{
	char			*nbr;
	unsigned int	num;
	size_t			len;

	num = n;
	len = ft_numlen(n);
	nbr = malloc(sizeof(char) * (len + 1));
	if (!nbr)
		return (NULL);
	if (num == 0)
	{
		nbr[0] = '0';
		nbr[1] = '\0';
		return (nbr);
	}
	ft_nbr_printer(nbr, num, len);
	return (nbr);
}
