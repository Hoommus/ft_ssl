/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/12 18:13:58 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static struct option	g_base_opts[] =
{
	{"q",        no_argument,        NULL, 'q'},
	{"r",        no_argument,        NULL, 'r'},
	{"p",        no_argument,        NULL, 'p'},
	{"s",        required_argument,  NULL, 's'},
	{NULL,       0,                  NULL,  0 }
};

static struct s_meta	g_md5_meta =
{
	.data_name = NULL,
	.flags = 0,
	.algo_type = MD5,
	.algo_name = ALGO_MD5,
	.data_size = 0,
	.is_string = false,
	.block_bit_size = 512,
	.digest_bit_size = 128,
};

static struct s_command	g_commands[] =
{
	{
		"md5", "MD5 message digest",
		MESSAGE_COMMAND, MD5,
		.cmd_executor = NULL,
		.iterative = &md5_iterative,
		.oneshot = &md5_oneshot,
		.options = g_base_opts
	},
	{
		"sha256", "SHA256 message digest",
		MESSAGE_COMMAND, SHA256,
		.cmd_executor = NULL,
		.iterative = NULL,
		.oneshot = NULL,
		.options = g_base_opts
	},
	{
		.name = "whirlpool",
		.description = "WHIRLPOOL message digest",
		.cmd_type = MESSAGE_COMMAND,
		.algo_type = WHIRLPOOL,
		.iterative = NULL,
		.oneshot = NULL,
		.options = g_base_opts
	},
	{
		.name = "help",
		.description = "Prints helpful messages",
		.cmd_type = GENERIC_COMMAND,
		.algo_type = TYPE_NOT_APPLICABLE,
		.cmd_executor = &help,
		.iterative = NULL,
		.oneshot = NULL,
		.options = g_base_opts
	},
	{
		.name = "exit",
		.description = "Prints helpful messages",
		.cmd_type = GENERIC_COMMAND,
		.algo_type = TYPE_NOT_APPLICABLE,
		.cmd_executor = &quit,
		.iterative = NULL,
		.oneshot = NULL,
		.options = NULL
	},
	{
		.name = "quit",
		.description = "Prints helpful messages",
		.cmd_type = HIDDEN_COMMAND,
		.algo_type = TYPE_NOT_APPLICABLE,
		.cmd_executor = &quit,
		.iterative = NULL,
		.oneshot = NULL,
		.options = NULL
	},
	{NULL, NULL, HIDDEN_COMMAND, TYPE_NOT_APPLICABLE, NULL, NULL, NULL, NULL}
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

void				(*get_processor(enum e_algo_type t, bool oneshot))(struct s_processable *)
{
	int		i;

	i = 0;
	while (g_commands[i].name)
		if (g_commands[i].algo_type == t)
			return (oneshot ? g_commands[i].oneshot : g_commands[i].iterative);
	return (NULL);
}

int					quit(char **args)
{
	*args = args[0];
	exit(1);
}

int					prepare_n_exec(struct s_command *cmd, char **args)
{
	struct s_processable	*generic;
	int						status;

	if (cmd->cmd_type == MESSAGE_COMMAND)
	{
		generic = ft_memalloc(sizeof(struct s_message));
		generic->magic = MAGIC_MESSAGE;
	}
	else if (cmd->cmd_type == CIPHER_COMMAND)
	{
		generic = ft_memalloc(sizeof(struct s_cipher));
		generic->magic = MAGIC_CIPHER;
	}
	else
		return (-1);
	generic->meta = g_md5_meta;
	status = process(generic, args, cmd->options);
	free(generic);
	return (status);
}

int					execute(char **args)
{
	int		i;

	i = 0;
	while (g_commands[i].name != NULL)
	{
		if (ft_strcmp(args[0], g_commands[i].name) == 0)
		{
			if (g_commands[i].algo_type != TYPE_NOT_APPLICABLE)
				return (prepare_n_exec(g_commands + i, args));
			return (g_commands[i].cmd_executor(args + 1));
		}
		i++;
	}
	return (-128);
}
