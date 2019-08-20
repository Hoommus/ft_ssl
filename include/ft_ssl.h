/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/20 16:17:31 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <stdint.h>
# include <stddef.h>
# include <getopt.h>
# include "libft.h"
# include "ft_printf.h"
# include "get_next_line.h"

# define ABS(a) ((a) < 0 ? -(a) : (a))
# define ISQT(x) (((x) == '\'' || (x) == '"' || (x) == '`') ? (1) : (0))
# define ISFLAG(arg) ((arg) && (arg)[0] == '-' && (arg)[1] != '\0')

# define F_QUIET        1U
# define F_REVERSE      2U
# define F_ECHO         4U
# define F_STRING       8U
# define F_FH           128U

# define ALGO_MD5       "MD5"
# define ALGO_SHA256    "SHA256"
# define ALGO_WHIRLPOOL "WHIRLPOOL"

# define MAGIC_MESSAGE  0xCAFE
# define MAGIC_CIPHER   0x1337

enum					e_algo_type
{
	TYPE_NOT_APPLICABLE = 0,
	TYPE_MD5 = 1,
	TYPE_SHA256 = 2,
	TYPE_WHIRLPOOL = 3,
};

enum					e_cmd_type
{
	STANDARD_COMMAND,
	CIPHER_COMMAND,
	MESSAGE_COMMAND,
	GENERIC_COMMAND,
	HIDDEN_COMMAND
};

struct					s_meta
{
	char				*data_name;
	const char			*algo_name;
	enum e_algo_type	algo_type;
	u_int8_t			flags;
	__uint128_t			data_size;
	__uint128_t			whole_size;
	size_t				block_bit_size;
	size_t				digest_bit_size;
};

typedef struct			s_processable
{
	u_int64_t		magic;
	struct s_meta	meta;
	u_int8_t		*data;
	u_int8_t		pad[64];
	u_int8_t		pad2[16];
}						t_processable;

struct					s_message
{
	u_int64_t		magic;
	struct s_meta	meta;
	u_int8_t		*data;
	u_int64_t		hash[8];
	__uint128_t		bit_size;
};

struct					s_cipher
{
	u_int64_t		magic;
	struct s_meta	meta;
	u_int8_t		*data;

};

struct					s_command
{
	char				*name;
	char				*description;
	enum e_cmd_type		cmd_type;
	enum e_algo_type	algo_type;
	int					(*cmd_executor)(char **args);
	void				(*iterative)(struct s_processable *generic);
	void				(*oneshot)(struct s_processable *generic);
	struct s_meta		meta;
	struct option		*options;
};

void					(*g_get_processor(enum e_algo_type t, bool oneshot))
	(struct s_processable *);

int						execute(char **args);
int						process(t_processable *generic, char **argv,
	struct option *longopts);
int						resolve_args(t_processable *generic, char **argv,
							struct option *const longopts);
/*
** commands
*/

int						quit(char **args);
int						help(char **args);

char					**smart_split(char *str, const char *delimiters);
void					strip_quotes(char **array);
int						read_fd(int fd, char **result, __uint128_t *size);

/*
** output.c
*/

void					print_usage(const char *algo_name);
void					print_digest(unsigned char *digest, size_t size);
void					print_digest_from_msg(struct s_message *msg);
void					print_error(char *cause, char *error);

/*
** universal_operations.c
*/

u_int32_t				swap_endianess(u_int32_t x);
u_int64_t				swap_endianess_64(u_int64_t x);
int						get_argc(char **argv);

/*
** MD5 (md5/algorithm.c)
*/

void					md5_oneshot(struct s_processable *const generic);
void					md5_iterative(struct s_processable *const generic);

/*
** SHA256
*/

void					sha256_oneshot(struct s_processable *const generic);
void					sha256_iterative(struct s_processable *const generic);

/*
** WHIRLPOOL
*/

void					whirlpool_oneshot(struct s_processable *generic);
void					whirlpool_iterative(struct s_processable *generic);
void					whirlpool_process_chunk(struct s_message *msg);

#endif
