/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/10 17:32:27 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/12 20:35:08 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#define GET_ARGC(v) ({int p = 0; char *const*r = (v); while(r && r[p]) p++; p;})

int					process_iterative(int fd, t_processable *generic)
{
	const size_t	to_read = generic->meta.block_bit_size / 8;
	int				status;

	generic->data = (u_int8_t *)ft_strnew(to_read);
	while ((status = read(fd, generic->data, to_read)) > 0)
	{
		generic->meta.data_size += status;
		get_processor(generic->meta.algo_type, false)(generic);
		ft_bzero(generic->data, to_read * sizeof(char));
	}
	free(generic->data);
	if (generic->magic == MAGIC_MESSAGE)
		print_digest_from_msg((struct s_message *)generic);
	else
		ft_printf("Cipher goes here\n");
	return (0);
}

void				resolve_echo(t_processable *generic)
{
	if (generic->meta.flags & F_ECHO)
		generic->data = (u_int8_t *) ft_strnew(0);
	else
		read_fd(0, (char **)&(generic->data), &(generic->meta.data_size));
	generic->meta.flags |= F_ECHO;
	get_processor(generic->meta.algo_type, true)(generic);
	write(1, generic->data, generic->meta.data_size);
	print_digest_from_msg((struct s_message *)generic);
	ft_memdel((void **)&(generic->data));
}

int					resolve_args(t_processable *generic, char *const *argv,
									struct option *const longopts)
{
	const int	argc = GET_ARGC(argv);
	int			ch;
	int			status;

	status = 0;
	while ((ch = getopt_long(argc, argv, "qrps:", longopts, NULL)) != -1)
		if (ch == 'q')
			generic->meta.flags |= F_QUIET;
		else if (ch == 'r')
			generic->meta.flags |= F_REVERSE;
		else if (ch == 'p')
			resolve_echo(generic);
		else if (ch == 's')
		{
			generic->data = (u_int8_t *)ft_strdup(optarg);
			generic->meta.data_name = optarg;
			generic->meta.data_size = ft_strlen(optarg);
			get_processor(generic->meta.algo_type, true)(generic);
			print_digest_from_msg((struct s_message *)generic);
			free(generic->data);
		}
		else
		{
			print_usage(generic->meta.algo_name);
			status = 1;
		}
	return (status);
}

int					process(t_processable *processable, char **argv,
							   struct option *longopts)
{
	int			argc;
	struct stat	s;
	int			fd;
	int			status;

	status = resolve_args(processable, argv, longopts);
	argc = GET_ARGC(argv) - optind + 1;
	argv += optind;
	while (--argc >= 0)
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
		}
		argv++;
	}
	return (status);
}