# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/24 10:11:17 by vtarasiu          #+#    #+#              #
#    Updated: 2019/08/20 16:11:25 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl

FLAGS = -std=c11 -Wall -Wextra -Werror \
		-g3 -Wno-unknown-pragmas

HEADER = include -I $(LIB_DIR)/include -I libft
SRC_DIR = ./src/
OBJ_DIR = ./obj/

LIB_DIR = ./printf
LIB_NAME = libftprintf.a

MAIN_SRC = main.c split/quotes.c split/smart_split.c executor.c \
           errors.c output.c parsing.c universal_operations.c \
           processor.c processor_args.c

MD5_DIR = md5/
MD5_SRC = algorithm.c

SHA256_DIR = sha256/
SHA256_SRC = algorithm.c

WHIRLPOOL_DIR = whirlpool/
WHIRLPOOL_SRC = whirlpool.c \
                whirlpool_sbox.c \
                whirlpool_blocks.c

OBJ = $(addprefix $(OBJ_DIR), $(MAIN_SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)$(MD5_DIR), $(MD5_SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)$(SHA256_DIR), $(SHA256_SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)$(WHIRLPOOL_DIR), $(WHIRLPOOL_SRC:.c=.o))

all: $(NAME)

$(NAME): prepare $(OBJ)
	make -C $(LIB_DIR)
	cp $(LIB_DIR)/$(LIB_NAME) ./$(LIB_NAME)
	gcc $(FLAGS) -o $(NAME) $(OBJ) -I $(HEADER) $(LIB_NAME)

prepare:
	@mkdir -p $(OBJ_DIR)$(WHIRLPOOL_DIR)
	@mkdir -p $(OBJ_DIR)$(SHA256_DIR)
	@mkdir -p $(OBJ_DIR)$(MD5_DIR)
	@mkdir -p $(OBJ_DIR)split

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	gcc $(FLAGS) -I $(HEADER) -o $@ -c $< ;

clean:
	make -C libft clean
	@echo "rm -rf $(OBJ_DIR)*.o"
	@/bin/rm -rf $(OBJ)
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIB_DIR) fclean
	/bin/rm -f $(NAME)
	/bin/rm -f $(LIB_NAME)

re: fclean $(NAME)

love:
	@echo "Not all."

.PHONY: clean all fclean re love
