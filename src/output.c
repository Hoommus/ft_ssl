/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 13:23:09 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/18 13:50:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void		print_digest(unsigned char *digest, size_t size)
{
	int		i;

	i = 0;
	while (i < size)
		ft_printf("%2.2x", digest[i++]);
}

void		print_file_hash(char *digest, char *data_name, char *algo_name)
{
	ft_printf("%s (%s) = %s", algo_name, data_name, digest);
}


void		print_str_hash(char *digest, char *data_name, char *algo_name)
{
	ft_printf("%s (\"%s\") = %s", algo_name, data_name, digest);
}

void		print_quiet(char *digest)
{
	ft_printf("%s", digest);
}

void		print_reverse(char *digest, char *data_name, char *algo_name)
{
	ft_printf("%s", digest);
}
