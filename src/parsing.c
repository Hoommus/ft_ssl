/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 11:10:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/30 13:08:44 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int			apply_flag(struct s_meta *meta, char c)
{
	if (c == 'q')
		meta->flags |= F_QUIET;
	else if (c == 'r')
		meta->flags |= F_REVERSE;
	else
	{
		ft_dprintf(2, "ft_ssl: -- %c is an invalid argument.\n", c);
		if (meta->algo_name != NULL)
			print_usage(meta->algo_name);
		return (-1);
	}
	return (0);
}

int			choose_operation(struct s_message *msg, char **args, int s)
{
	int i;
	int off;

	off = -1;
	while (s != -1 && (msg->meta->flags & F_FH) != F_FH && ++off != -42
		&& ISFLAG(args[off]) && !(i = 0))
		while (ISFLAG(args[off]) && args[off][++i])
			if (args[off][i] == 's' && args[off][i + 1] == '\0')
			{
				arg_string(msg, args[++off]);
				break ;
			}
			else if (args[off][i] == 's' && args[off][i + 1] != '\0')
			{
				arg_string(msg, args[off] + i + 1);
				break ;
			}
			else if (args[off][i] == 'p')
			{
				stdin_echo(msg);
				msg->meta->flags |= F_ECHO;
			}
			else if (apply_flag(msg->meta, args[off][i]) != 0)
				return (-2);
	return (off);
}

int			read_filename(char *file, char **data, size_t *size)
{
	int			fd;
	struct stat	s;

	stat(file, &s);
	if (!S_ISREG(s.st_mode))
	{
		print_error(file, "Is not a regular file or such file does not exist.");
		return (1);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		print_error(file, strerror(errno));
		return (1);
	}
	read_fd(fd, data, size);
	return (0);
}

int			read_fd(int fd, char **result, size_t *size)
{
	char		buffer[1025];
	char		*data;
	char		*swap;
	int			status;

	ft_bzero(buffer, 1025);
	*size = 0;
	data = ft_strnew(0);
	while ((status = read(fd, buffer, 1024)) > 0)
	{
		swap = ft_tolerant_strjoin(data, *size, buffer, status);
		*size += status;
		free(data);
		data = swap;
		ft_bzero(buffer, 1025);
	}
	*result = data;
	return ((int)*size);
}

int			read_arg_str(char *arg, char **data, size_t *size)
{
	if (arg == NULL)
	{
		print_error(NULL, "option requires string argument -- s");
		return (1);
	}
	*data = ft_strdup(arg);
	*size = ft_strlen(*data);
	return (0);
}
