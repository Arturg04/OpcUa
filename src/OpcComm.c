/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcComm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:43:20 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/12 16:03:46 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/OpcLib.h"

void	connection_inactivity_callback(UA_Client *client)
{

}

void	connection_state_callback(UA_Client *client,
						UA_SecureChannelState chanelstate ,
						UA_SessionState sessionstate,
						UA_StatusCode connectstatus)
{
	t_TagInfo	*tags;
	char	*url;

	if (sessionstate == UA_CONNECTIONSTATE_OPENING)
		printf("Connection Open!\n");
	else if (sessionstate == UA_CONNECTIONSTATE_CLOSED)
	{
		printf("Server disconnected. Attempting to reconnect...\n");
		url = (char	*)&UA_Client_getConfig(client)->applicationUri;
		if (!*url)
			return ;
		connectstatus = UA_Client_connect(client, url);
		while (connectstatus != UA_STATUSCODE_GOOD)
		{
			printf("Failed to reconnect.\n");
			connectstatus = UA_Client_connect(client, url);
		}
	}
	if (connectstatus == UA_STATUSCODE_GOOD && sessionstate == UA_SESSIONSTATE_ACTIVATED && chanelstate == UA_SECURECHANNELSTATE_OPEN)
	{
		printf("Connection Established.\n");
		tags = (t_TagInfo *)UA_Client_getConfig(client)->clientContext;
		while (tags)
			tags = create_subscription(client, tags);
	}
}

UA_Client	*connect_opc_server(const char *serverUrl, t_TagInfo *tags)
{
	UA_Client		*client;
	UA_StatusCode	retval;

	client = UA_Client_new();
	UA_ClientConfig_setDefault(UA_Client_getConfig(client));
	UA_Client_getConfig(client)->stateCallback = connection_state_callback;
	UA_Client_getConfig(client)->inactivityCallback = connection_inactivity_callback;
	UA_Client_getConfig(client)->requestedSessionTimeout = 0;
	UA_Client_getConfig(client)->timeout = 10;
	UA_Client_getConfig(client)->connectivityCheckInterval = 10;
	UA_Client_getConfig(client)->clientContext = tags;
	retval = UA_Client_connect(client, serverUrl);
	while (retval != UA_STATUSCODE_GOOD)
	{
		printf("Failed to reconnect.\n");
		retval = UA_Client_connect(client, serverUrl);
	}
	return (client);
}

void	disconnect_opc_server(UA_Client *client)
{
	if (client != NULL)
	{
		UA_Client_disconnect(client);
		UA_Client_delete(client);
		printf("Disconnected from the server.\n");
	}
}