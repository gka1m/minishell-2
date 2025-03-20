/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkaim <gkaim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:08:35 by zchan             #+#    #+#             */
/*   Updated: 2025/03/07 14:16:49 by gkaim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*define error codes to have more meaning or use enums*/

//example use definition macros
#define ERR_PIPE 1
#define ERR_FORK 2
#define ERR_EXEC 3

int report_error(const char *msg, int code)
{
    printf("%s\n", msg);
    return (code);
}
