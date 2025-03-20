/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_2d_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchan <zchan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 09:04:30 by zchan             #+#    #+#             */
/*   Updated: 2024/10/11 17:38:09 by zchan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Ensure the array itself is valid
// Free each element if it's valid
// Prevent accidental reuse of the freed memory
// Free the main array itself
// Set the original pointer to NULL to avoid dangling pointers
void	ft_free_2d_array(void **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		if (array[i])
		{
			free(array[i]);
			array[i] = NULL;
		}
		i++;
	}
	free(array);
	array = NULL;
}

// // Free each individual row in the array
// // Avoid dangling pointers
// // Free the array itself
// // This line has no effect on the original pointer;
// // ignored in practice
// void	ft_free_2d_array(void **array)
// {
// 	int	i;

// 	if (!array)
// 		return ;
// 	i = 0;
// 	while (array[i])
// 	{
// 		free(array[i]);
// 		array[i] = NULL;
// 		i++;
// 	}
// 	free(array);
// 	array = NULL;
// }

// Ensure the array pointer is valid
// Free each element in the array
// Avoid dangling pointers
// Free the array itself
// Set the caller's pointer to NULL to prevent use-after-free
// void	ft_free_2d_array(void ***array)
// {
// 	int	i;

// 	if (!array || !*array)
// 		return ;
// 	i = 0;
// 	while ((*array)[i])
// 	{
// 		free((*array)[i]);
// 		(*array)[i] = NULL;
// 		i++;
// 	}
// 	free(*array);
// 	*array = NULL;
// }

// void	ft_free_2d_array(void **array)
// {
// 	int	i;

// 	i = 0;
// 	if (!array || !*array)
// 		return ;
// 	while (array[i])
// 	{
// 		free(array[i]);
// 		i++;
// 	}
// 	free(array);
// 	array = NULL;
// }

// void free_2d_array(void ***array)
// {
//     if (array && *array)
//     {
//         for (int i = 0; (*array)[i]; i++)
//         {
//             free((*array)[i]);
//         }
//         free(*array);
//         *array = NULL; // Set the pointer to NULL to avoid dangling pointers
//     }
// }