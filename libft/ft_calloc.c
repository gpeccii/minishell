/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbordin <rbordin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 10:29:58 by rbordin           #+#    #+#             */
/*   Updated: 2023/05/30 15:26:58 by rbordin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n);

void	*ft_calloc(size_t count, size_t size)
{
	void	*a;

	if (count == SIZE_MAX && size == SIZE_MAX)
		return (NULL);
	a = (void *)malloc(count * size);
	if (a == NULL)
		return (NULL);
	ft_bzero(a, (count * size));
	return (a);
}
