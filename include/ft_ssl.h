/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 11:36:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/12 18:15:03 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <getopt.h>
# include "libft.h"
# include "ft_printf.h"
# include "get_next_line.h"

# define ABS(a) (a < 0 ? -(a) : a)
# define ISQT(x) (((x) == '\'' || (x) == '"' || (x) == '`') ? (1) : (0))
# define ISFLAG(arg) ((arg) && (arg)[0] == '-' && (arg)[1] != '\0')

# define F_QUIET        1U
# define F_REVERSE      2U
# define F_ECHO         4U
# define F_FH           128U

# define ALGO_MD5       "MD5"
# define ALGO_SHA256    "SHA256"
# define ALGO_WHIRLPOOL "WHIRLPOOL"

# define MAGIC_MESSAGE  0xCAFE
# define MAGIC_ITERATOR 0xBABE
# define MAGIC_CIPHER   0x1337

enum					e_algo_type
{
	MD5,
	SHA256,
	WHIRLPOOL,
	TYPE_NOT_APPLICABLE
};

enum					e_cmd_type
{
	STANDARD_COMMAND,
	CIPHER_COMMAND,
	MESSAGE_COMMAND,
	GENERIC_COMMAND,
	HIDDEN_COMMAND
};

struct			s_meta
{
	char				*data_name;
	const char			*algo_name;
	enum e_algo_type	algo_type;
	u_int8_t			is_string;
	u_int8_t			flags;
	size_t				data_size;
	size_t				block_bit_size;
	size_t				digest_bit_size;
};

typedef struct			s_processable
{
	u_int64_t		magic; // 8 bytes
	struct s_meta	meta;  // 46 bytes
	u_int8_t		*data; // 8 bytes
	                       // --- 62 bytes
	u_int8_t		__pad[64];
}						t_processable;

struct					s_message
{
	u_int64_t		magic;
	struct s_meta	meta;
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
} __attribute__((aligned(16)));

struct					s_cipher
{
	u_int64_t		magic;
	struct s_meta	meta;
	u_int8_t		*data;

};

struct					s_iterator
{
	u_int64_t		magic;
	int				fd;
	t_processable	*processable;
	void			(*padder)(t_processable *this);
	void			(*chunker)(t_processable *this);
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
	struct option		*options;
};

void					(*get_processor(enum e_algo_type t, bool oneshot))
	(struct s_processable *);

int						execute(char **args);
int						process(t_processable *processable, char **argv,
	 struct option *longopts);
int						quit(char **args);
int						help(char **args);

char					**smart_split(char *str, const char *delimiters);
void					strip_quotes(char **array);
int						read_fd(int fd, char **data, size_t *size);

/*
** output.c
*/

void					print_usage(const char *algo_name);
void					print_digest(unsigned char *digest, size_t size);
void print_digest_from_msg(struct s_message *msg);
void					print_error(char *cause, char *error);

/*
** universal_operations.c
*/

u_int32_t				swap_endianess(u_int32_t x);

/*
** MD5 (md5/algorithm.c)
*/

void					md5_oneshot(struct s_processable *const msg);
void					md5_iterative(struct s_processable *message);

/*
** SHA256
*/

int						sha256(char **args);
void					sha256_process_message(struct s_message *msg);

/*
** Auxiliary
*/

const char				*algo_name(enum e_algo_type type);

#endif
