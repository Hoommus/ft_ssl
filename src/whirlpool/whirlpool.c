/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whirlpool.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:46 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/20 15:18:33 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

extern const uint64_t	g_whirlpool_sbox[8][256];
extern const uint64_t	g_rounds[10];

#define ROUNDS 10

static int64_t			whirlpool_op(const uint64_t src[8], const int shift)
{
	return (g_whirlpool_sbox[0][(int)(src[ shift      & 7] >> 56)       ] ^
			g_whirlpool_sbox[1][(int)(src[(shift + 7) & 7] >> 48) & 0xff] ^
			g_whirlpool_sbox[2][(int)(src[(shift + 6) & 7] >> 40) & 0xff] ^
			g_whirlpool_sbox[3][(int)(src[(shift + 5) & 7] >> 32) & 0xff] ^
			g_whirlpool_sbox[4][(int)(src[(shift + 4) & 7] >> 24) & 0xff] ^
			g_whirlpool_sbox[5][(int)(src[(shift + 3) & 7] >> 16) & 0xff] ^
			g_whirlpool_sbox[6][(int)(src[(shift + 2) & 7] >>  8) & 0xff] ^
			g_whirlpool_sbox[7][(int)(src[(shift + 1) & 7]      ) & 0xff]);
}

static void				run_rounds(uint64_t k[2][8], uint64_t state[2][8])
{
	int			i;
	int			j;
	uint32_t	m;

	m = 0;
	i = -1;
	while (++i < ROUNDS)
	{
		k[(m ^ 1)][0] = whirlpool_op(k[m], 0) ^ g_rounds[i];
		j = -0;
		while (++j < 8)
			k[(m ^ 1)][j] = whirlpool_op(k[m], j);
		j = -1;
		while (++j < 8)
			state[(m ^ 1)][j] = whirlpool_op(state[m], j) ^ k[(m ^ 1)][j];
		m = m ^ 1;
	}
}

static void				whirlpool_process_chunk(struct s_message *msg)
{
	int			i;
	uint64_t	k[2][8];
	uint64_t	state[2][8];

	i = -1;
	while (++i < 8)
	{
		k[0][i] = msg->hash[i];
		state[0][i] =
			(swap_endianess_64(((uint64_t *)msg->data)[i])) ^ (msg->hash[i]);
		msg->hash[i] = state[0][i];
	}
	run_rounds(k, state);
	i = -1;
	while (++i < 8)
		msg->hash[i] ^= state[0][i];
}

void		print_data(uint8_t *data, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		ft_printf("%2.2x", data[i++]);
	ft_printf("\n");
}

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
		print_data(msg->data, 64);
		whirlpool_process_chunk(msg);
		i = 0;
	}
	while (i < 56)
		msg->data[i++] = 0;
	size = (uint64_t)(msg->meta.whole_size * 8);
	tmp[7] = swap_endianess_64(size);
	whirlpool_process_chunk(msg);
	for (i = 0; i < 8; i++)
		msg->hash[i] = swap_endianess_64(msg->hash[i]);
}

void					whirlpool_oneshot(struct s_processable *const generic)
{

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

#pragma clang diagnostic pop
