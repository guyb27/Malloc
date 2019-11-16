# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2019/10/14 23:36:24 by gmadec       #+#   ##    ##    #+#        #
#    Updated: 2019/10/21 23:41:55 by gmadec      ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so
FLAGS = # -Werror -Wextra -Wall
MALLOC_PATH = #./sources/
MALLOC = malloc.c realloc.c utils.c free.c bonus.c init_and_print.c utils_bis.c malloc_bis.c free_bis.c
SRCS = $(MALLOC_PATH)$(MALLOC)
OBJ = $(MALLOC_PATH)$(MALLOC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@gcc -shared -o $@ $^
	@ln  -fs  $(NAME)  libft_malloc.so
	@echo "\033[33;32m[√]"

$(DIR_OBJ)%.o: $(DIR_SRC)%.c $(INC)
	@mkdir -p objets
	@gcc -g $(FLAGS) -o $@ -c $< -I includes
	@echo "\033[1;32m[OK]\033[0m" $<

clean:
	@echo "Cleaning objets."
	@rm -rf objets

fclean: clean
	@echo "Cleaning $(NAME) and libft_malloc_darwin.so."
	@rm -rf $(NAME)
	@rm -rf $(OBJ)
	@rm -rf libft_malloc_$(HOSTTYPE).so
	@rm -rf libft_malloc.so

re: fclean all
