/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxilary.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/07 16:47:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/07 17:12:35 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

const static char	*g_algo_names[] =
{
	"MD5",
	"SHA256",
	"WHIRLPOOL"
};

const char			*algo_name(enum e_algo_type type)
{
	if (type > sizeof(g_algo_names) / 8)
		return ("algo_name");
	return (g_algo_names[type]);
}

