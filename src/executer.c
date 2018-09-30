/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/30 11:11:29 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

struct s_command	g_commands[] =
{
	{
		"md5", "MD5 message digest",
		MESSAGE_COMMAND, MD5,
		&md5, &md5_process_message
	},
	{
		"sha256", "SHA256 message digest",
		MESSAGE_COMMAND, SHA256,
		&sha256, &sha256_process_message
	},
	{
		"help", "Prints helpful messages",
		GENERIC_COMMAND, WHIRLPOOL, &help, NULL
	},
	{"exit", "quit", GENERIC_COMMAND, WHIRLPOOL, &quit, NULL},
	{"quit", "quit", HIDDEN_COMMAND, WHIRLPOOL, &quit, NULL},
	{NULL, NULL, HIDDEN_COMMAND, WHIRLPOOL, NULL, NULL}
};

int					help(char **args)
{
	int		i;

	*args = args[0];
	ft_printf("Message commands:\n");
	i = -1;
	while (g_commands[++i].name != NULL)
		if (g_commands[i].cmd_type == MESSAGE_COMMAND)
			ft_printf("%s ", g_commands[i].name);
	i = -1;
	ft_printf("\nCipher commands: \n");
	while (g_commands[++i].name != NULL)
		if (g_commands[i].cmd_type == CIPHER_COMMAND)
			ft_printf("%s ", g_commands[i].name);
	i = -1;
	ft_printf("\nGeneric commands: \n");
	while (g_commands[++i].name != NULL)
		if (g_commands[i].cmd_type == GENERIC_COMMAND)
			ft_printf("%s ", g_commands[i].name);
	ft_printf("\n");
	return (0);
}

int					quit(char **args)
{
	*args = args[0];
	exit(1);
}

/*
** Returns function of type 'void (*) (struct s_message *)'
*/

t_message_processor	get_message_processor(enum e_algo_type algo_type)
{
	int		i;

	i = 0;
	while (g_commands[i].name != NULL)
	{
		if (g_commands[i].algo_type == algo_type)
			return (g_commands[i].message_processor);
		i++;
	}
	return (NULL);
}

int					execute(char **args)
{
	int		i;

	i = 0;
	while (g_commands[i].name != NULL)
	{
		if (ft_strcmp(args[0], g_commands[i].name) == 0)
			return (g_commands[i].entry(args + 1));
		i++;
	}
	return (2);
}
