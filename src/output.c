/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 13:23:09 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/12 19:31:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void		print_usage(const char *algo_name)
{
	char	lower[512];

	ft_strcpy(lower, algo_name);
	ft_tolowercase(lower);
	ft_printf("usage: ft_ssl %s [-pqr] [-s string] [files ...]\n", lower);
}

void		print_message(struct s_message *msg, char *input)
{
	size_t	i;

	ft_printf("%s", input);
	i = 0;
	while (i < msg->bit_size / 8)
		ft_printf("%2.2x", msg->data[i++]);
}

void print_digest_from_msg(struct s_message *msg)
{
	u_int8_t	*c;

	c = (u_int8_t *)&(msg->a);
	if ((msg->meta.flags & F_QUIET) != F_QUIET &&
		(msg->meta.flags & F_REVERSE) != F_REVERSE)
	{
		if (msg->meta.flags & F_STRING)
			ft_printf("%s (\"%s\") = ", msg->meta.algo_name,
					msg->meta.data_name);
		else
			ft_printf("%s (%s) = ", msg->meta.algo_name, msg->meta.data_name);
	}
	else if ((msg->meta.flags & F_ECHO) == F_ECHO)
	{
		msg->data[msg->meta.data_size] = '\0';
		ft_printf("%s", msg->data);
	}
	print_digest(c, msg->meta.digest_bit_size / 8);
	if ((msg->meta.flags & F_QUIET) != F_QUIET &&
		(msg->meta.flags & F_REVERSE) == F_REVERSE)
		ft_printf(" %s ", msg->meta.data_name);
	ft_printf("\n");
}

#include <stdio.h>

void		print_digest(u_int8_t *digest, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		printf("%2.2x", digest[i++]);
	fflush(stdout);
}
