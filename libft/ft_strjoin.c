/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 16:43:50 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/29 17:12:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	size_t			len;
	char			*new;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (*s1)
		new[i++] = *s1++;
	while (*s2)
		new[i++] = *s2++;
	new[len] = '\0';
	return (new);
}

char	*ft_tolerant_strjoin(char const *s1, size_t size1,
							char const *s2, size_t size2)
{
	size_t		new_len;
	char		*new;
	size_t		i;
	size_t		j;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	new_len = size1 + size2;
	new = ft_strnew(new_len);
	i = 0;
	j = 0;
	while (i < size1)
		new[j++] = s1[i++];
	i = 0;
	while (i < size2)
		new[j++] = s2[i++];
	return (new);
}
