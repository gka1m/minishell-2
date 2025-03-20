/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_expansion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kagoh <kagoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:58:14 by kagoh             #+#    #+#             */
/*   Updated: 2025/03/20 16:03:52 by kagoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void    print_output(int mode, int fd, char **line, t_minishell *shell)
{
    char    *temp;

    temp = NULL;
    if (mode == 0)
        temp = expand_hd(*line, shell);
    else
        temp = ft_strdup(*line);
    write(fd, temp, ft_strlen(temp));
    write(fd, "\n", 1);
    (free(*line), free(temp));
}

