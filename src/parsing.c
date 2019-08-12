/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 11:10:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/12 18:05:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

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

