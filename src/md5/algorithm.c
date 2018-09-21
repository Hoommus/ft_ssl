/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:46 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/19 16:48:28 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static const uint32_t		g_shifts[64] =
{
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static const uint32_t		g_keys[64] =
{
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
	0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
	0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
	0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
	0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
	0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static const uint32_t		g_magic_initial[4] =
{
	0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476
};

struct s_message	*preprocess(char *str)
{
	uint32_t			initial_len;
	struct s_message	*msg;

	msg = ft_memalloc(sizeof(struct s_message));
	initial_len = ft_strlen(str);
	msg->bit_size = initial_len * 8 + 1;
	while (msg->bit_size % 512 != 448)
		msg->bit_size++;
	msg->data = (uint8_t *)ft_strnew((msg->bit_size + 64) / 8);
	ft_memcpy(msg->data, str, initial_len);
	msg->data[initial_len] = 128;
	initial_len = initial_len * 8;
	ft_memcpy(msg->data + msg->bit_size / 8, &initial_len, 4);
	//msg->bit_size += 64;
	return (msg);
}

void				loop_body(const struct s_message *msg, const uint32_t j,
													uint32_t *f, uint32_t *g)
{
	if (j < 16)
	{
		*f = (msg->b & msg->c) | ((~msg->b) & msg->d);
		*g = j;
	}
	else if (j < 32)
	{
		*f = (msg->d & msg->b) | ((~msg->d) & msg->c);
		*g = (5 * j + 1) % 16;
	}
	else if (j < 48)
	{
		*f = (msg->b ^ msg->c ^ msg->d);
		*g = (3 * j + 5) % 16;
	}
	else
	{
		*f = msg->c ^ (msg->b | (~msg->d));
		*g = (7 * j) % 16;
	}
}

void				process_chunk(struct s_message *msg, size_t offset)
{
	uint32_t		*chunk;
	uint32_t		f;
	uint32_t		g;
	uint32_t		j;

	j = 0;
	chunk = (uint32_t *)(msg->data + offset);
	while (j < 64)
	{
		loop_body(msg, j, &f, &g);
		f += msg->a + g_keys[j] + chunk[g];
		msg->a = msg->d;
		msg->d = msg->c;
		msg->c = msg->b;
		msg->b = msg->b + ROTATE_LEFT(f, g_shifts[j]);
		j++;
	}
}

void				process_message(struct s_message *msg)
{
	uint32_t	magic[4];
	uint32_t	i;

	i = 0;
	magic[0] = g_magic_initial[0];
	magic[1] = g_magic_initial[1];
	magic[2] = g_magic_initial[2];
	magic[3] = g_magic_initial[3];
	while (i < msg->bit_size / 8)
	{
		msg->a = magic[0];
		msg->b = magic[1];
		msg->c = magic[2];
		msg->d = magic[3];
		process_chunk(msg, i);
		magic[0] += msg->a;
		magic[1] += msg->b;
		magic[2] += msg->c;
		magic[3] += msg->d;
		i += 64;
	}
	msg->a = magic[0];
	msg->b = magic[1];
	msg->c = magic[2];
	msg->d = magic[3];
}

int					md5(char **args)
{
	struct s_message	*msg;
	char				*data;
	char				flags;
	uint32_t			digest_words[4];
	uint8_t				digest[16];

	msg = preprocess(args[0]);
	process_message(msg);
	digest_words[0] = msg->a;
	digest_words[1] = msg->b;
	digest_words[2] = msg->c;
	digest_words[3] = msg->d;
	ft_memcpy(digest, digest_words, sizeof(uint32_t) * 4);
	print_digest();
	return (0);
}
