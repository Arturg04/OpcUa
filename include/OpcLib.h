/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcLib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:30:46 by Arturg04          #+#    #+#             */
/*   Updated: 2023/10/16 09:17:07 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPCLIB_H
# define OPCLIB_H
# ifdef _WIN32
#  include <winsock2.h>
#  include <windows.h>
# else
#  include <unistd.h>
# endif
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <sqlite3.h>
# include <errno.h>
# include "../Open62541_files/open62541.h"
# include "libft.h"



# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# define MAX_FILES 1024
# define MAX_TAGS_PER_SUB 20

typedef struct s_node {
	int	index;
	int	identifier;
}	t_node;

typedef struct s_TagInfo {
	char				*name;
	t_node				*node;
	char				*tag_type;
	int32_t				*subid;
	struct s_TagInfo	*next;
	struct s_TagInfo	*prev;
}	t_TagInfo;


// Server Comm
UA_Client	*connect_opc_server(const char *serverUrl, t_TagInfo *tags);
void		disconnect_opc_server(UA_Client *client);
t_TagInfo	*create_subscription(UA_Client *client, t_TagInfo *Tag);

//xml
t_TagInfo	*readxml(char *filepath);

//sqlite
int			open_db(char *filepath);
int			create_table(void);
int			insert_data(t_TagInfo *tag, int value);
int			update_data(t_TagInfo *tag, int value);
int			delete_data(t_TagInfo *tag);
void		close_db(void);

#endif
