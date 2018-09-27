/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/19 15:41:26 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include "libft.h"
# include "ft_printf.h"
# include "get_next_line.h"

# define ABS(a) (a < 0 ? -(a) : a)
# define ISQT(x) (((x) == '\'' || (x) == '"' || (x) == '`') ? (1) : (0))
# define ISFLAG(arg) ((arg) && (arg)[0] == '-' && (arg)[1] != '\0')

# define ROTATE_LEFT(x, c)  (((x) << (c)) | ((x) >> (32 - (c))))
# define ROTATE_RIGHT(x, c) (((x) >> (c)) | ((x) << (32 - (c))))

# define F_QUIET    1
# define F_REVERSE  2
# define F_ECHO     4
# define F_HASHED   128

# define ALGO_MD5       "MD5"
# define ALGO_SHA256    "SHA256"
# define ALGO_WHIRLPOOL "WHIRLPOOL"

enum					e_algo_type
{
	MD5,
	SHA256,
	WHIRLPOOL
};

enum					e_cmd_type
{
	STANDARD_COMMAND,
	CIPHER_COMMAND,
	MESSAGE_COMMAND,
	GENERIC_COMMAND,
	HIDDEN_COMMAND
};

struct					s_command
{
	char			*name;
	char			*description;
	enum e_cmd_type	type;
	int				(*function) (char **args);
};

struct					s_meta
{
	char				*data_name;
	char				*algo_name;
	enum e_algo_type	algo_type;
	size_t				data_size;
	u_int8_t			is_string;
	u_int8_t			flags;
};

struct					s_message
{
	struct s_meta	*meta;
	u_int8_t		*data;
	u_int32_t		a;
	u_int32_t		b;
	u_int32_t		c;
	u_int32_t		d;
	u_int32_t		e;
	u_int32_t		f;
	u_int32_t		g;
	u_int32_t		h;
	size_t			bit_size;
	size_t			byte_size;
};

extern struct s_command	g_commands[];
extern struct s_command	g_algorithms[];

/*
** Execute, cannot pardon? Or...
** Execute cannot, pardon?
** Got it?
*/

int						execute(char **args);
int						quit(char **args);
int						help(char **args);
int						choose_operation(struct s_message *msg, char **args);

char					**smart_split(char *str, const char *delimiters);
int						read_fd(int fd, char **data, size_t *size);
int						read_arg_str(char *arg, char **data, size_t *size);
int						read_filename(char *file, char **data, size_t *size);
/*
** output.c
*/
void					print_usage(char *algo_name);
void					print_digest(unsigned char *digest, size_t size);
void					print_digest_from_msg(struct s_message *msg,
														size_t digest_size);
void					print_error(char *cause, char *error);

/*
** universal_operations.c
*/
void					stdin_echo(struct s_message *msg);
void					arg_string(struct s_message *msg, char *str);
void					op_file(struct s_message *msg, char *filename);

/*
 * MD5 -qs
 */
void					process_message_md5(struct s_message *msg);
int						md5(char **args);

/*
 * SHA256
 */

#endif
