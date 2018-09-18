/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/18 18:03:23 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include "libft.h"
# include "ft_printf.h"
# include "get_next_line.h"
# include <fcntl.h>

# define ABS(a) (a < 0 ? -(a) : a)
# define ISQT(x) (((x) == '\'' || (x) == '"' || (x) == '`') ? (1) : (0))

enum					e_cmd_type
{
	STANDARD_COMMAND,
	CIPHER_COMMAND,
	MESSAGE_COMMAND,
	HULLO
};

struct					s_command
{
	char			*name;
	char			*description;
	enum e_cmd_type	type;
	int				(*function) (char **args);
};

struct					s_message
{
	uint8_t			*data;
	size_t			initial_size;
	size_t			bit_size;
};

extern struct s_command	g_commands[];

/*
** Execute, cannot pardon? Or...
** Execute cannot, pardon?
** Got it?
*/

int						execute(char **args);

char					**smart_split(char *str, const char *delimiters);

/*
 * MD5
 */
int						md5(char **args);

/*
 * SHA256
 */

#endif
