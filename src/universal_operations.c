#include "ft_ssl.h"

void		stdin_echo(struct s_message *msg)
{
	if ((msg->meta->flags & F_ECHO) != F_ECHO)
	{
		if (!read_fd(0, (char **) &(msg->data), &(msg->byte_size)))
			return;
	}
	else
		msg->data = (u_int8_t *)ft_strnew(0);
	process_message_md5(msg);
	msg->meta->flags |= F_QUIET;
	print_digest_from_msg(msg, sizeof(u_int32_t) * 4);
	msg->meta->flags &= ~F_QUIET;
	chfree(msg->meta->data_name);
	chfree(msg->data);
}

void		arg_string(struct s_message *msg, char *str)
{
	if (!read_arg_str(str, (char **)&(msg->data), &(msg->byte_size)))
	{
		msg->meta->is_string = 1;
		msg->meta->data_name = ft_strdup((char *)msg->data);
		process_message_md5(msg);
		print_digest_from_msg(msg, sizeof(u_int32_t) * 4);
		chfree(msg->meta->data_name);
		chfree(msg->data);
		msg->meta->is_string = 0;
	}
}

void		op_file(struct s_message *msg, char *filename)
{
	if (!read_filename(filename, (char **)&(msg->data), &(msg->byte_size)))
	{
		msg->meta->data_name = ft_strdup(filename);
		process_message_md5(msg);
		print_digest_from_msg(msg, sizeof(u_int32_t) * 4);
		chfree(msg->meta->data_name);
		chfree(msg->data);
	}
}
