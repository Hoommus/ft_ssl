/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whirlpool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:46 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/20 16:01:04 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static void				whirlpool_pad(struct s_message *msg)
{
	uint64_t		tmp[8];
	uint64_t		size;
	__uint128_t		i;

	ft_bzero(tmp, sizeof(tmp));
	ft_memcpy(tmp, msg->data, msg->meta.data_size);
	msg->data = (uint8_t *)tmp;
	i = msg->meta.data_size;
	msg->data[i++] = 128;
	if (i > 32)
	{
		while (i < 64)
			msg->data[i++] = 0;
		whirlpool_process_chunk(msg);
		i = 0;
	}
	while (i < 56)
		msg->data[i++] = 0;
	size = (uint64_t)(msg->meta.whole_size * 8);
	tmp[7] = swap_endianess_64(size);
	whirlpool_process_chunk(msg);
	i = -1;
	while (++i < 8)
		msg->hash[i] = swap_endianess_64(msg->hash[i]);
}

void					whirlpool_oneshot(struct s_processable *const generic)
{
	const void			*backup = generic->data;
	struct s_message	*msg;
	uint64_t			i;

	msg = (struct s_message *)generic;
	i = 0;
	while (i < generic->meta.whole_size)
	{
		if (generic->meta.whole_size - i < 64)
		{
			whirlpool_pad(msg);
			break ;
		}
		whirlpool_process_chunk(msg);
		msg->data += 64;
		i += 64;
	}
	generic->data = (uint8_t *)backup;
}

void					whirlpool_iterative(struct s_processable *const generic)
{
	struct s_message	*msg;
	uint8_t				*swap;

	msg = (struct s_message *)generic;
	if (msg->meta.data_size < msg->meta.block_bit_size / 8)
	{
		swap = msg->data;
		whirlpool_pad(msg);
		msg->data = swap;
		return ;
	}
	whirlpool_process_chunk(msg);
}
