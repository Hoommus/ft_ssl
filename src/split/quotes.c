/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:58 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/18 11:36:58 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

/*
** Function strips first and last quote from given string.
*/
char	*remove_quotes(char *string)
{
	char	q;
	size_t	len;

	q = string[0];
	len = ft_strlen(string);
	if (len >= 2 && ISQT(q) && string[len - 1] == q)
	{
		string[0] = 0;
		string[len - 1] = 0;
		ft_memmove(string, string + 1, len - 2);
	}
	return (string);
}
