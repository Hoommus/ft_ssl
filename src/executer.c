/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/18 13:20:16 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

struct s_command	g_commands[] =
{
	{"md5", "MD5 message digest", MESSAGE_COMMAND, &md5},
	{"sha256", "SHA256 message digest", MESSAGE_COMMAND, &md5},

	{NULL, NULL, HULLO, NULL}
};

int					execute(char **args)
{
	int		i;

	i = 0;
	while (g_commands[i].name != NULL)
	{
		if (ft_strcmp(args[0], g_commands[i].name) == 0)
			return (g_commands[i].function(args));
		i++;
	}
	return (1);
}
