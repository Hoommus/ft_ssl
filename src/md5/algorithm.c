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

static const u_int32_t		g_shifts[64] =
{
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static const u_int32_t		g_keys[64] =
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

static const u_int32_t		g_magic_initial[4] =
{
	0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476
};

struct s_message	*preprocess(struct s_message *const msg)
{
	u_int32_t			byte_len;
	u_int8_t			*swap;

	msg->bit_size = msg->byte_size * 8;
	msg->meta->data_size = msg->byte_size;
	byte_len = msg->bit_size / 8;
	msg->bit_size += 1;
	while (msg->bit_size % 512 != 448)
		msg->bit_size++;
	swap = (u_int8_t *)ft_strnew((msg->bit_size + 64) / 8);
	ft_memcpy(swap, msg->data, byte_len);
	free(msg->data);
	msg->data = swap;
	msg->data[byte_len] = 128;
	byte_len = byte_len * 8;
	ft_memcpy(msg->data + msg->bit_size / 8, &byte_len, 4);
	msg->bit_size += 64;
	return (msg);
}

void				loop_body(const struct s_message *msg, const u_int32_t j,
													u_int32_t *f, u_int32_t *g)
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
	u_int32_t		*chunk;
	u_int32_t		f;
	u_int32_t		g;
	u_int32_t		j;

	j = 0;
	chunk = (u_int32_t *)(msg->data + offset);
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

void				process_message_md5(struct s_message *msg)
{
	u_int32_t	magic[4];
	u_int32_t	i;

	i = 0;
	preprocess(msg);
	ft_memcpy(magic, g_magic_initial, sizeof(u_int32_t) * 4);
	ft_memcpy(&(msg->a), magic, sizeof(u_int32_t) * 4);
	while (i < msg->bit_size / 8)
	{
		process_chunk(msg, i);
		magic[0] += msg->a;
		magic[1] += msg->b;
		magic[2] += msg->c;
		magic[3] += msg->d;
		ft_memcpy(&(msg->a), magic, sizeof(u_int32_t) * 4);
		i += 64;
	}
}

int					md5(char **args)
{
	struct s_message	*msg;
	int					i;
	int					s;

	i = 0;
	msg = (struct s_message *)ft_memalloc(sizeof(struct s_message));
	msg->meta = (struct s_meta *)ft_memalloc(sizeof(struct s_meta));
	msg->meta->algo_name = ALGO_MD5;
	while (i != -1)
	{
		s = choose_operation(msg, args + i);
		i += s == -1 ? 0 : s;
		if ((s == 0 && args[i] == NULL))
			stdin_echo(msg);
		else if (args[i] != NULL)
			op_file(msg, args[i]);
		ft_bzero(&(msg->data), sizeof(struct s_message)
							- sizeof(struct s_meta *));
		if (args[i] == NULL || args[++i] == NULL)
			break ;
	}
	chfree_n(2, msg->meta, msg);
	return (0);
}
