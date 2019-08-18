/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/10 17:32:27 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/13 19:15:08 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int			get_argc(char **argv)
{
	int		i;

	i = 0;
	while (argv && argv[i])
		i++;
	return (i);
}

int			process_iterative(int fd, t_processable *generic)
{
	const size_t	to_read = generic->meta.block_bit_size / 8;
	void			(*processor)(struct s_processable *);
	int				status;

	processor = get_processor(generic->meta.algo_type, false);
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

void		resolve_string(t_processable *generic)
{
	generic->meta.flags |= F_STRING;
	generic->data = (u_int8_t *)ft_strdup(optarg);
	generic->meta.data_name = optarg;
	generic->meta.whole_size = ft_strlen(optarg);
	generic->meta.data_size = generic->meta.whole_size;
	get_processor(generic->meta.algo_type, true)(generic);
	print_digest_from_msg((struct s_message *)generic);
	ft_memdel((void **)&(generic->data));
}

void		resolve_echo(t_processable *generic)
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

int			resolve_args(t_processable *generic, char **argv,
									struct option *const longopts)
{
	const int	argc = get_argc(argv);
	int			ch;
	int			status;

	optind = 0;
	optreset = 1;
	status = 0;
	while ((ch = getopt_long(argc, argv, "qrps:", longopts, NULL)) != -1)
		if (ch == 'q')
			generic->meta.flags |= F_QUIET;
		else if (ch == 'r')
			generic->meta.flags |= F_REVERSE;
		else if (ch == 'p')
			resolve_echo(generic);
		else if (ch == 's')
			resolve_string(generic);
		else
		{
			print_usage(generic->meta.algo_name);
			status = 1;
		}
	return (status);
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
		get_processor(generic->meta.algo_type, true)(generic);
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