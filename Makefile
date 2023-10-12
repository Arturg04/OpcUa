# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/12 16:06:55 by ade-pinh          #+#    #+#              #
#    Updated: 2023/10/12 16:22:36 by ade-pinh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC_Linux = cc

CC_Windows = i686-w64-mingw32-gcc

INCLUDE = include

LIB = lib/libft.a -lsqlite3

OPEN62541 = Open62541_files/open62541.o

CFLAGS = -Wall -Wextra -Werror

PRESRCS = src/

SRC = Database.c OpcComm.c OpcSub.c readfile.c OpcService.c

SRCS = $(addprefix $(PRESRCS), $(SRC))

OBJS = $(SRC:.c.o)

Name = OpcService

all: $(Name)

$(Name): $(OBJS)
	$(CC_Linux) $(OPEN62541) $(LIB) $(CFLAGS) -o $(Name).out $(OBJS)
	$(CC_Windows) $(OPEN62541) $(LIB) $(CFLAGS) -o $(Name).exe $(OBJS)

%.o: %.c
	$(CC_Linux) $(CFLAGS) -c $< -o $@
	$(CC_Windows) $(CFLAGS) -c $< -o $@.exe

$(Name):

clean:
	rm -f $(OBJS) $(OBJS:=.exe)

fclean: clean
	rm -f $(Name).out $(Name).exe

re: fclean all


