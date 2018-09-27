/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/14 17:21:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/14 17:21:57 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

ssize_t	goats_teleported(void)
{
	ssize_t	goats;
	int		fd;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (1);
	else
	{
		read(fd, &goats, sizeof(ssize_t));
		close(fd);
		if (goats == 0)
			goats += 1348;
		return (ABS(goats));
	}
}

char	*wait_for_input(void)
{
	char	*line;

	while (goats_teleported())
	{
		if (get_next_line(STDIN_FILENO, &line) > 0)
		{
			if (ft_strlen(line) == 0 && line != NULL)
			{
				free(line);
				return (NULL);
			}
			return (line);
		}
	}
	return (NULL);
}

void	shell_loop(void)
{
	char	*command;
	char	**args;

	while (goats_teleported())
	{
		ft_printf("\x1b[37;1mft_SSL>\x1b[0m ");
		command = wait_for_input();
		if (command == NULL)
			continue;
		args = smart_split(command, LIBFT_WHTSP);
		if (execute(args) == 2)
			print_error(args[0], "is an invalid command.");
		free(command);
		free_array(args);
	}
}

int		main(int argc, char **argv)
{
	if (argc == 1)
		shell_loop();
	else if (execute(argv + 1) == 2)
		print_error(argv[1], "is an invalid command.");
}
