# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: gmadec <marvin@le-101.fr>                  +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2019/09/17 11:04:56 by gmadec       #+#   ##    ##    #+#        #
#    Updated: 2019/09/17 14:59:05 by gmadec      ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

build: srcs/main.c srcs/test.c
		ifeq ($(HOSTTYPE),)
			HOSTTYPE := $(shell uname -m)_$(shell uname -s)
		endif
	     gcc -o ft_malloc srcs/main.c srcs/test.c -I include
