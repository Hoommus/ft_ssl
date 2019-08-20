/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   universal_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 11:12:07 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/19 19:59:35 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

u_int32_t	swap_endianess(u_int32_t x)
{
	x = ((x << 8UL) & 0xFF00FF00UL) | ((x >> 8UL) & 0xFF00FFUL);
	return (x << 16UL) | (x >> 16UL);
}

u_int64_t	swap_endianess_64(u_int64_t x)
{
	return (__builtin_bswap64(x));
}
