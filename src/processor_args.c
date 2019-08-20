/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/20 16:09:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/20 16:11:16 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void		resolve_string(t_processable *generic)
{
	generic->meta.flags |= F_STRING;
	generic->data = (u_int8_t *)ft_strdup(optarg);
	generic->meta.data_name = optarg;
	generic->meta.whole_size = ft_strlen(optarg);
	generic->meta.data_size = generic->meta.whole_size;
	g_get_processor(generic->meta.algo_type, true)(generic);
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
	g_get_processor(generic->meta.algo_type, true)(generic);
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
