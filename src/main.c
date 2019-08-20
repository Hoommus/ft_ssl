/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/14 17:21:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/20 16:06:09 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <stdnoreturn.h>

char				*wait_for_input(void)
{
	char	*line;
	int		status;

	while (true)
		if ((status = get_next_line(STDIN_FILENO, &line)) > 0)
		{
			if (ft_strlen(line) == 0 && line != NULL)
			{
				free(line);
				return (NULL);
			}
			return (line);
		}
		else if (status == 0)
		{
			write(1, "exit\n", 5);
			exit(0);
		}
		else if (status == -1)
			print_error("stdin", strerror(errno));
}

#define VOID_OF_NORETURN noreturn void

VOID_OF_NORETURN	shell_loop(void)
{
	char	*command;
	char	**args;

	while (true)
	{
		ft_printf("\x1b[37;1mft_ssl>\x1b[0m ");
		command = wait_for_input();
		if (command == NULL)
			continue ;
		args = smart_split(command, LIBFT_WHTSP);
		strip_quotes(args);
		if (execute(args) == -128)
			print_error(args[0], "is an invalid command.");
		free(command);
		free_array(args);
	}
}

int					main(int argc, char **argv)
{
	ft_printf("%lu\n", sizeof(struct s_message));
	ft_printf("%lu\n", sizeof(struct s_processable));
	if (argc == 1)
		shell_loop();
	else if (execute(argv + 1) == -128)
		print_error(argv[1], "is an invalid command.");
}
