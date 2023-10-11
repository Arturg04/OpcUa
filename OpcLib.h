/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcLib.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:30:46 by Arturg04          #+#    #+#             */
/*   Updated: 2023/10/11 14:31:45 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPCLIB_H
# define OPCLIB_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include "Open62541_files/open62541.h"
# include "libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# define MAX_FILES 1024

typedef	struct s_node
{
	int	index;
	int	identifier;
} t_node;

typedef struct s_TagInfo {
	char				*name;
	t_node				*node;
	char				*tag_type;
	int32_t				*subid;
	struct s_TagInfo	*next;
	struct s_TagInfo	*prev;
}	t_TagInfo;


// Server Comm
UA_Client	*ConnectToOpcServer(const char *serverUrl);
void	DisconnectFromOpcServer(UA_Client *client);
void	CreateSubscription(UA_Client *client, t_TagInfo *Tag);



//xml
t_TagInfo	*readxml(char *filepath);


#endif
