# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/12 16:06:55 by ade-pinh          #+#    #+#              #
#    Updated: 2023/10/16 12:31:04 by ade-pinh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC_Linux = cc -g
CC_Windows = x86_64-w64-mingw32-gcc -g
INCLUDE = -I include
INCLUDEWIN = -I /usr/x86_64-w64-mingw32/include/ -I include -lws2_32 -liphlpapi
LIB = -lsqlite3 -ljansson
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)

LIBFT_DIR = lib/Libft
LIBFT = $(wildcard $(LIBFT_DIR)/*.c)

OPEN62541_DIR = Open62541_files
OPEN62541 = $(wildcard $(OPEN62541_DIR)/*.c)

OBJ_DIR_LIN = src/lin
OBJ_DIR_WIN = src/win

OBJS_LIN = $(addprefix $(OBJ_DIR_LIN)/, $(SRC:$(SRC_DIR)/%.c=%.o) $(LIBFT:$(LIBFT_DIR)/%.c=%.o) $(OPEN62541:$(OPEN62541_DIR)/%.c=%.o))
OBJS_WIN = $(addprefix $(OBJ_DIR_WIN)/, $(SRC:$(SRC_DIR)/%.c=%.o) $(LIBFT:$(LIBFT_DIR)/%.c=%.o) $(OPEN62541:$(OPEN62541_DIR)/%.c=%.o))

Name = bin/OpcService

all: dir lin win

dir:
	mkdir -p $(OBJ_DIR_LIN)
	mkdir -p $(OBJ_DIR_WIN)
	mkdir -p bin
	cp dependencies bin
	cp config.json bin
	cp tags.json bin

# LINUX
lin: $(OBJS_LIN)
	$(CC_Linux) $(OBJS_LIN) $(CFLAGS) $(INCLUDE) $(LIB) -o $(Name).out -W1,-rpath, '$$ORIGIN/bin/dependencies'

# WINDOWS
win: $(OBJS_WIN)
	$(CC_Windows) $(OBJS_WIN) $(CFLAGS) $(INCLUDEWIN) $(LIB) -o $(Name).exe -W1,-rpath,'$$ORIGIN/bin/dependencies'

$(OBJ_DIR_LIN)/%.o: $(SRC_DIR)/%.c
	$(CC_Linux) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR_WIN)/%.o: $(SRC_DIR)/%.c
	$(CC_Windows) $(CFLAGS) $(INCLUDEWIN) -c $< -o $@

$(OBJ_DIR_LIN)/%.o: $(LIBFT_DIR)/%.c
	$(CC_Linux) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR_WIN)/%.o: $(LIBFT_DIR)/%.c
	$(CC_Windows) $(CFLAGS) $(INCLUDEWIN) -c $< -o $@

$(OBJ_DIR_LIN)/%.o: $(OPEN62541_DIR)/%.c
	$(CC_Linux) $(INCLUDE) -c $< -o $@

$(OBJ_DIR_WIN)/%.o: $(OPEN62541_DIR)/%.c
	$(CC_Windows) $(INCLUDEWIN) -c $< -o $@

clean:
	rm -f -r $(OBJ_DIR_LIN)/*.o $(OBJ_DIR_WIN)/*.o $(OBJ_DIR_LIN) $(OBJ_DIR_WIN)

fclean: clean
	rm -f -r $(Name).out $(Name).exe IEM.db bin
.PHONY: all dir lin win clean fclean re
