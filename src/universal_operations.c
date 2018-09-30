/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   universal_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 11:12:07 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/30 12:56:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

u_int32_t	swap_endianess(u_int32_t x)
{
	x = ((x << 8) & 0xFF00FF00) | ((x >> 8) & 0xFF00FF);
	return (x << 16) | (x >> 16);
}

int			get_digest_size(enum e_algo_type t)
{
	if (t == MD5)
		return (4);
	else if (t == SHA256)
		return (8);
	else if (t == WHIRLPOOL)
		return (8);
	else
		return (1);
}

void		stdin_echo(struct s_message *msg)
{
	if ((msg->meta->flags & F_ECHO) != F_ECHO)
	{
		if (!read_fd(0, (char **)&(msg->data), &(msg->byte_size)))
			return ;
	}
	else
		msg->data = (u_int8_t *)ft_strnew(0);
	get_message_processor(msg->meta->algo_type)(msg);
	msg->meta->flags |= F_QUIET;
	print_digest_from_msg(msg, sizeof(u_int32_t) *
								get_digest_size(msg->meta->algo_type));
	msg->meta->flags &= ~F_QUIET;
	chfree(msg->meta->data_name);
	chfree(msg->data);
	msg->meta->data_name = NULL;
	msg->data = NULL;
}

void		arg_string(struct s_message *msg, char *str)
{
	if (!read_arg_str(str, (char **)&(msg->data), &(msg->byte_size)))
	{
		msg->meta->is_string = 1;
		msg->meta->data_name = ft_strdup((char *)msg->data);
		get_message_processor(msg->meta->algo_type)(msg);
		print_digest_from_msg(msg, sizeof(u_int32_t) *
									get_digest_size(msg->meta->algo_type));
		chfree(msg->meta->data_name);
		chfree(msg->data);
		msg->meta->is_string = 0;
		msg->meta->data_name = NULL;
		msg->data = NULL;
	}
}

void		op_file(struct s_message *msg, char *filename)
{
	if (!read_filename(filename, (char **)&(msg->data), &(msg->byte_size)))
	{
		msg->meta->data_name = ft_strdup(filename);
		get_message_processor(msg->meta->algo_type)(msg);
		print_digest_from_msg(msg, sizeof(u_int32_t) *
									get_digest_size(msg->meta->algo_type));
		chfree(msg->meta->data_name);
		chfree(msg->data);
		msg->meta->flags |= F_FH;
		msg->meta->data_name = NULL;
		msg->data = NULL;
	}
}
