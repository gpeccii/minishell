/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpecci <gpecci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 16:47:20 by gpecci            #+#    #+#             */
/*   Updated: 2023/01/23 18:59:29 by gpecci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	if (!src && !dst)
		return (0);
	if (dstsize == 0)
		return (ft_strlen(src));
	i = 0;
	j = 0;
	while (dst[j] && j < dstsize)
		j++;
	while ((i + j + 1) < dstsize && src[i])
	{
		dst[i + j] = src[i];
		i++;
	}
	if (j != dstsize)
		dst[i + j] = '\0';
	return (j + ft_strlen(src));
}
