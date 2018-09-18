# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/24 10:11:17 by vtarasiu          #+#    #+#              #
#    Updated: 2018/09/14 17:23:57 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl

FLAGS = -std=c99 -Wall -Wextra -Werror

HEADER = include -I $(LIB_DIR)/include -I libft
SRC_DIR = ./src/
OBJ_DIR = ./obj/

LIB_DIR = ./printf
LIB_NAME = libftprintf.a

MAIN_SRC = main.c split/quotes.c split/smart_split.c memory.c

MD5_DIR = ./md5/
MD5_SRC = parsing.c

SHA256_DIR = ./sha256/
SHA256_SRC = parsing.c

OBJ = $(addprefix $(OBJ_DIR), $(MAIN_SRC:.c=.o)) \
      $(addprefix $(MD5_DIR), $(MD5_SRC:.c=.o))

all: $(NAME)

$(NAME): prepare $(OBJ)
	make -C $(LIB_DIR)
	cp $(LIB_DIR)/$(LIB_NAME) ./$(LIB_NAME)
	gcc $(FLAGS) -o $(NAME) $(OBJ) -I $(HEADER) $(LIB_NAME) -ltermcap

prepare:
	@mkdir -p $(OBJ_DIR)$(MD5_DIR)
	@mkdir -p $(OBJ_DIR)$(BUILTIN_DIR)
	@mkdir -p $(OBJ_DIR)$(INTERFACE_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	gcc $(FLAGS) -I $(HEADER) -o $@ -c $< ;

clean:
	make -C libft clean
	@echo "rm -rf $(OBJ_DIR)*.o"
	@/bin/rm -rf $(OBJ)
	/bin/rm -rf $(OBJ_DIR)

norme:
	@echo "<<<===========>>>"
	norminette $(wildcard $(SRC_DIR)*.c)
	@echo "<<<===========>>>"
	norminette $(HEADER)
	@echo "<<<===========>>>"
	norminette $(wildcard $(LIB_DIR)/src/*.c)
	@echo "<<<===========>>>"
	norminette $(wildcard $(LIB_DIR)/include/*.h)
	@echo "<<<===========>>>"
	norminette $(wildcard ./libft/*.c)
	@echo "<<<===========>>>"
	norminette $(wildcard ./libft/*.h)

fclean: clean
	make -C $(LIB_DIR) fclean
	/bin/rm -f $(NAME)
	/bin/rm -f $(LIB_NAME)

re: fclean $(NAME)

love:
	@echo "Not all."

.PHONY: clean all fclean re love norme
