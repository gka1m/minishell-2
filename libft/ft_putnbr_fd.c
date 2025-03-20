/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:56:38 by zchan             #+#    #+#             */
/*   Updated: 2024/05/28 15:33:26 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putnbr_rec(unsigned int i, int fd)
{
	if (i / 10 != 0)
	{
		ft_putnbr_rec(i / 10, fd);
	}
	ft_putchar_fd(i % 10 + '0', fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	nbr;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		nbr = -n;
	}
	else
	{
		nbr = n;
	}
	ft_putnbr_rec(nbr, fd);
}
