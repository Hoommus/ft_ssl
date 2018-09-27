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
	{"help", "Prints helpful messages", GENERIC_COMMAND, &help},
	{"exit", "quit", GENERIC_COMMAND, &quit},
	{"quit", "quit", HIDDEN_COMMAND, &quit},
	{NULL, NULL, GENERIC_COMMAND, NULL}
};

int					help(char **args)
{
	int		i;

	*args = args[0];
	ft_printf("Message commands:\n");
	i = -1;
	while (g_commands[++i].name != NULL)
		if (g_commands[i].type == MESSAGE_COMMAND)
			ft_printf("%s ", g_commands[i].name);
	i = -1;
	ft_printf("\nCipher commands: \n");
	while (g_commands[++i].name != NULL)
		if (g_commands[i].type == CIPHER_COMMAND)
			ft_printf("%s ", g_commands[i].name);
	i = -1;
	ft_printf("\nGeneric commands: \n");
	while (g_commands[++i].name != NULL)
		if (g_commands[i].type == GENERIC_COMMAND)
			ft_printf("%s ", g_commands[i].name);
	ft_printf("\n");
	return (0);
}

int					quit(char **args)
{
	*args = args[0];
	exit(1);
}

int					execute(char **args)
{
	int		i;

	i = 0;
	while (g_commands[i].name != NULL)
	{
		if (ft_strcmp(args[0], g_commands[i].name) == 0)
			return (g_commands[i].function(args + 1));
		i++;
	}
	return (2);
}
