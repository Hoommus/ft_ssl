/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:54 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/12 18:19:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int			count_substrings(const char *const str, const char *delimiters)
{
	ssize_t	i;
	ssize_t	len;
	int		subs;
	char	c;

	len = ft_strlen(str);
	subs = 0;
	i = -1;
	while (++i < len)
	{
		c = str[i];
		if (ft_strchr(LIBFT_WHTSP, str[i]) != NULL && ++i)
			continue ;
		else if (ISQT(str[i]) && ++i)
			while (str[i] != 0 && str[i] != c)
				i++;
		else
			while (str[i] != 0 && ft_strchr(delimiters, str[i]) == NULL)
				i++;
		subs++;
	}
	return (subs);
}

/*
** Returns pointer to any found needle char in str.
*/

char		*ft_strchr_any(const char *str, const char *needles)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (needles[j])
			if (str[i] == needles[j++])
				return ((char *)str + i);
		i++;
	}
	return (NULL);
}

long long	get_word_size(const char *str, const char *delimiters)
{
	long long	i;
	char		c;

	c = *str;
	i = 0;
	if (ISQT(*str) && ++i)
		while (str[i] && str[i] != c)
			i++;
	else
		i = (long long)(ft_strchr_any(str, delimiters) - str);
	if (i < 0)
		return ((long long)ft_strlen(str));
	return (i);
}

/*
** Split that takes into account quotes ("", '', ``), separators - ';', '\n'
** and brackets
** TODO: Try to fix that too high memory allocation thing
** TODO: Try to fix not enough memory allocation
*/

char		**smart_split(char *str, const char *delimiters)
{
	char		**array;
	long long	j;
	long long	i;
	long long	word_size;
	long long	subs;

	array = (char **)ft_memalloc(sizeof(char *) *
				(subs = count_substrings(str, delimiters) + 2));
	j = 0;
	i = -1;
	while (str[++i] && j < subs - 1)
	{
		if (ft_strchr(delimiters, str[i]) != NULL)
			continue ;
		word_size = get_word_size(str + i, delimiters);
		array[j++] = ft_strsub(str + i, 0, word_size + ISQT(str[i]));
		if (str[i] != ';')
			i += word_size - !ISQT(str[i]);
	}
	array[j] = NULL;
	return (array);
}
