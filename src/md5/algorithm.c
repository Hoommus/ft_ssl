/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:46 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/18 18:57:11 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

uint32_t			g_steps[64] =
{
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	05, 9, 14, 20, 05, 9, 14, 20, 05, 9, 14, 20, 05, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

uint32_t			g_keys[64] =
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

int					md5(char **args)
{

	return (0);
}

struct s_message	*preprocess(char *str)
{
	uint64_t			initial_len;
	struct s_message	*msg;

	msg = ft_memalloc(sizeof(struct s_message));
	initial_len = ft_strlen(str) * sizeof(&str);
	msg->initial_size = initial_len;
	msg->bit_size = initial_len * 8 + 1;
	while (msg->bit_size % 512 != 448)
		msg->bit_size++;
	msg->data = (uint8_t *)ft_strnew(msg->bit_size / 8 + 8);
	ft_memcpy(msg->data, str, initial_len);
	msg->data[initial_len] = 128;
	ft_memcpy(msg->data + initial_len + 56, &initial_len, sizeof(initial_len));
	return (msg);
}

struct s_message	*init(struct s_message msg)
{

}

char				*get_checksum(char *data)
{
	int a0 = 0x67452301;
	int b0 = 0xefcdab89;
	int c0 = 0x98badcfe;
	int d0 = 0x10325476;



	return (NULL);
}
