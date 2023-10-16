/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcLib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:30:46 by Arturg04          #+#    #+#             */
/*   Updated: 2023/10/16 16:54:05 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPCLIB_H
# define OPCLIB_H
# define CONFIG_FILE "config.json"

# include <stdio.h>
# include <stdlib.h>
# include <jansson.h>
# include <string.h>
# include <signal.h>
# include "sqlite3.h"
# include <errno.h>
# include "open62541.h"
# include "libft.h"

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

//sqlite
int			open_db(char *filepath);
int			create_table(void);
int			insert_data(t_TagInfo *tag, int value);
int			update_data(t_TagInfo *tag, int value);
int			delete_data(t_TagInfo *tag);
void		close_db(void);

//json
t_TagInfo	*read_json(char *filepath);


#endif
