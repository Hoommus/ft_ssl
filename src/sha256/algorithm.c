/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:46 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/13 13:01:09 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#define ROR(x, c) (((x) >> (c)) | ((x) << (32 - (c))))

#define SHA256_CH(e, f, g) (((e) & (f)) ^ ((~(e)) & (g)))
#define SHA256_MJ(a, b, c) (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))

static const u_int32_t	g_keys[64] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static const u_int32_t	g_magic_initial[8] =
{
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

static struct s_message	*sha256_preprocess(struct s_message *const msg)
{
	const __uint128_t	bit_len = msg->meta.whole_size * 8;
	u_int32_t			i;
	u_int32_t			*padded;
	u_int64_t			chunks;

	asm("mov rdx, rax");
	msg->bit_size = bit_len;
	chunks = 1 + ((msg->bit_size + 16 + 64) / 512);
	padded = ft_memalloc(16 * chunks * 32);
	if (msg->meta.data_size < UINT64_MAX)
		ft_memcpy(padded, msg->data, msg->meta.data_size);
	free(msg->data);
	msg->data = (u_int8_t *)padded;
	msg->data[msg->meta.data_size] = 128;
	i = -1;
	while (++i < (chunks * 16) - 1)
		padded[i] = swap_endianess(padded[i]);
	padded[((chunks * 512 - 64) / 32) + 1] = bit_len;
	msg->bit_size += 64;
	return (msg);
}

static void				extend_words(u_int8_t *data, u_int32_t w[64])
{
	int		j;

	j = 15;
	ft_memcpy(w, (u_int32_t *)data, sizeof(u_int32_t) * 16);
	while (++j < 64)
		w[j] = (ROR(w[j - 15], 7) ^ ROR(w[j - 15], 18) ^ (w[j - 15] >> 3)) +
				(ROR(w[j - 2], 17) ^ ROR(w[j - 2], 19) ^ (w[j - 2] >> 10)) +
				w[j - 16] + w[j - 7];
}

static void				sha256_process_chunk(struct s_message *const msg,
																size_t offset)
{
	u_int32_t		w[64];
	u_int32_t		tmp1;
	u_int32_t		tmp2;
	u_int32_t		j;

	ft_bzero(w, sizeof(w));
	extend_words(msg->data + offset, w);
	j = 0;
	while (j < 64)
	{
		tmp1 = (ROR(msg->e, 6) ^ ROR(msg->e, 11) ^ ROR(msg->e, 25)) +
				SHA256_CH(msg->e, msg->f, msg->g) + msg->h + g_keys[j] + w[j];
		tmp2 = (ROR(msg->a, 2) ^ ROR(msg->a, 13) ^ ROR(msg->a, 22)) +
				SHA256_MJ(msg->a, msg->b, msg->c);
		msg->h = msg->g;
		msg->g = msg->f;
		msg->f = msg->e;
		msg->e = msg->d + tmp1;
		msg->d = msg->c;
		msg->c = msg->b;
		msg->b = msg->a;
		msg->a = tmp1 + tmp2;
		j++;
	}
}

void					sha256_oneshot(struct s_processable *const generic)
{
	struct s_message *const	msg = (struct s_message *)generic;
	u_int32_t				h[8];
	u_int32_t				i;

	i = 0;
	sha256_preprocess(msg);
	ft_memcpy(h, g_magic_initial, sizeof(u_int32_t) * 8);
	ft_memcpy(&(msg->a), h, sizeof(u_int32_t) * 8);
	while (i < msg->bit_size / 8)
	{
		sha256_process_chunk(msg, i);
		h[0] += msg->a;
		h[1] += msg->b;
		h[2] += msg->c;
		h[3] += msg->d;
		h[4] += msg->e;
		h[5] += msg->f;
		h[6] += msg->g;
		h[7] += msg->h;
		ft_memcpy(&(msg->a), h, sizeof(u_int32_t) * 8);
		i += 64;
	}
	i = -1;
	while (++i < 8)
		*(&(msg->a) + i) = swap_endianess(*(&(msg->a) + i));
}

void					sha256_iterative(struct s_processable *const generic)
{
	static u_int32_t		h[8];
	struct s_message		*msg;
	int						i;

	msg = (struct s_message *)generic;
	if (ft_memcmp(&(msg->a), &((unsigned int[8]){0}), 0) == 0)
		ft_memcpy(h, g_magic_initial, sizeof(u_int32_t) * 8);
	ft_memcpy(&(msg->a), h, sizeof(u_int32_t) * 8);
	if (msg->meta.data_size < (msg->meta.block_bit_size / 8))
		sha256_preprocess(msg);
	sha256_process_chunk(msg, 0);
	i = -1;
	while (++i < 8)
		h[i] += *(&(msg->a) + i);
	if (msg->meta.data_size < (msg->meta.block_bit_size / 8))
	{
		ft_memcpy(&(msg->a), h, sizeof(u_int32_t) * 8);
		i = -1;
		while (++i < 8)
			*(&(msg->a) + i) = swap_endianess(*(&(msg->a) + i));
	}
}
