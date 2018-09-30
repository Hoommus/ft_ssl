/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 13:23:09 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/30 12:19:50 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void		print_usage(char *algo_name)
{
	ft_printf("usage: ft_ssl %s [-pqr] [-s string] [files ...]\n",
			ft_tolowercase(algo_name));
	ft_touppercase(algo_name);
}

void		print_message(struct s_message *msg, char *input)
{
	size_t	i;

	ft_printf("%s", input);
	i = 0;
	while (i < msg->bit_size / 8)
		ft_printf("%2.2x", msg->data[i++]);
}

void		print_digest_from_msg(struct s_message *msg, size_t digest_size)
{
	unsigned char	*c;

	c = (u_int8_t *)&(msg->a);
	if ((msg->meta->flags & F_QUIET) != F_QUIET &&
		(msg->meta->flags & F_REVERSE) != F_REVERSE)
	{
		if (msg->meta->is_string)
			ft_printf("%s (\"%s\") = ", msg->meta->algo_name,
					msg->meta->data_name);
		else
			ft_printf("%s (%s) = ", msg->meta->algo_name, msg->meta->data_name);
	}
	else if ((msg->meta->flags & F_ECHO) == F_ECHO)
	{
		msg->data[msg->meta->data_size] = '\0';
		ft_printf("%s", msg->data);
	}
	print_digest(c, digest_size);
	if ((msg->meta->flags & F_QUIET) != F_QUIET &&
		(msg->meta->flags & F_REVERSE) == F_REVERSE)
		ft_printf(" %s ", msg->meta->data_name);
	ft_printf("\n");
}

void		print_digest(unsigned char *digest, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		ft_printf("%2.2x", digest[i++]);
}
