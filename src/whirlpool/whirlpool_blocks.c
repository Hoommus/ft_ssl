/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whirlpool_blocks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 16:01:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/20 16:01:06 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

extern const uint64_t	g_whirlpool_sbox[8][256];
extern const uint64_t	g_rounds[10];

#define ROUNDS 10

static int64_t			whirlpool_op(const uint64_t src[8], const int shift)
{
	return (g_whirlpool_sbox[0][(int)(src[shift & 7] >> 56)] ^
			g_whirlpool_sbox[1][(int)(src[(shift + 7) & 7] >> 48) & 0xff] ^
			g_whirlpool_sbox[2][(int)(src[(shift + 6) & 7] >> 40) & 0xff] ^
			g_whirlpool_sbox[3][(int)(src[(shift + 5) & 7] >> 32) & 0xff] ^
			g_whirlpool_sbox[4][(int)(src[(shift + 4) & 7] >> 24) & 0xff] ^
			g_whirlpool_sbox[5][(int)(src[(shift + 3) & 7] >> 16) & 0xff] ^
			g_whirlpool_sbox[6][(int)(src[(shift + 2) & 7] >> 8) & 0xff] ^
			g_whirlpool_sbox[7][(int)(src[(shift + 1) & 7]) & 0xff]);
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

void					whirlpool_process_chunk(struct s_message *msg)
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
