/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/10 17:32:27 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/20 16:09:36 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int			process_iterative(int fd, t_processable *generic)
{
	const size_t	to_read = generic->meta.block_bit_size / 8;
	void			(*processor)(struct s_processable *);
	int				status;

	processor = g_get_processor(generic->meta.algo_type, false);
	generic->data = (u_int8_t *)ft_strnew(to_read);
	while ((status = read(fd, generic->data, to_read)) > 0)
	{
		generic->meta.data_size = status;
		generic->meta.whole_size += status;
		processor(generic);
		ft_bzero(generic->data, to_read * sizeof(char));
	}
	free(generic->data);
	if (generic->magic == MAGIC_MESSAGE)
		print_digest_from_msg((struct s_message *)generic);
	else
		ft_printf("Cipher goes here\n");
	return (0);
}

int			resolve_files(t_processable *processable, int argc, char **argv)
{
	struct stat	s;
	int			fd;
	int			status;

	status = 0;
	while (--argc > 0)
	{
		if ((stat(*argv, &s) == -1) || (fd = open(*argv, O_RDONLY)) == -1)
		{
			status = 2;
			print_error(*argv, strerror(errno));
		}
		else
		{
			processable->meta.data_name = *argv;
			status = status == 0 ? process_iterative(fd, processable) : status;
			processable->meta.data_name = NULL;
			close(fd);
		}
		argv++;
	}
	return (status);
}

int			process(t_processable *generic, char **argv,
	struct option *longopts)
{
	int			argc;
	int			status;

	status = 0;
	if (argv[0] == NULL || argv[1] == NULL)
	{
		generic->meta.flags |= F_QUIET;
		read_fd(0, (char **)&(generic->data), &generic->meta.whole_size);
		generic->meta.data_size = generic->meta.whole_size;
		g_get_processor(generic->meta.algo_type, true)(generic);
		print_digest_from_msg((struct s_message *)generic);
		ft_memdel((void **)&(generic->data));
	}
	else
	{
		status = resolve_args(generic, argv, longopts);
		argc = get_argc(argv) - optind + 1;
		argv += optind;
		status = resolve_files(generic, argc, argv) == 0 ? status : 8;
	}
	return (status);
}
