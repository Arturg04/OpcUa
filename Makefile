# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/12 16:06:55 by ade-pinh          #+#    #+#              #
#    Updated: 2023/10/13 17:09:14 by ade-pinh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC_Linux = cc
CC_Windows = x86_64-w64-mingw32-gcc
INCLUDE = -I "include"
INCLUDEWIN = -I "/usr/x86_64-w64-mingw32/include" -I "include"
LIB = -lsqlite3
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

Name = OpcService

all: dir lin win

dir:
	mkdir -p $(OBJ_DIR_LIN)
	mkdir -p $(OBJ_DIR_WIN)

# LINUX
lin: $(OBJS_LIN)
	$(CC_Linux) $(OBJS_LIN) $(CFLAGS) $(INCLUDE) $(LIB) -o $(Name).out

# WINDOWS
win: $(OBJS_WIN)
	$(CC_Windows) $(OBJS_WIN) $(CFLAGS) $(INCLUDEWIN) $(LIB) -o $(Name).exe

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
	rm -f $(OBJ_DIR_LIN)/*.o $(OBJ_DIR_WIN)/*.o

fclean: clean
	rm -f $(Name).out $(Name).exe

re: fclean all

.PHONY: all dir lin win clean fclean re
