/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   universal_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 11:12:07 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/12 18:05:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

u_int32_t	swap_endianess(u_int32_t x)
{
	x = ((x << 8UL) & 0xFF00FF00UL) | ((x >> 8UL) & 0xFF00FFUL);
	return (x << 16UL) | (x >> 16UL);
}

int			get_digest_size(enum e_algo_type t)
{
	if (t == TYPE_MD5)
		return (4);
	else if (t == TYPE_SHA256)
		return (8);
	else if (t == TYPE_WHIRLPOOL)
		return (8);
	else
		return (1);
}
