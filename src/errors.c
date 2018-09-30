/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 11:10:26 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/30 11:10:26 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void		print_error(char *cause, char *error)
{
	if (cause != 0)
		ft_dprintf(STDERR_FILENO, "ft_ssl: %s: %s\n", cause, error);
	else
		ft_dprintf(STDERR_FILENO, "ft_ssl: %s\n", error);
}
