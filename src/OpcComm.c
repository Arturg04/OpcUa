/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcComm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:43:20 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/12 17:23:50 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/OpcLib.h"

static UA_Boolean	g_running = true;
static int			g_server = 0;
static char			*g_url;

static void	stop_handler(int signal)
{
	(void)signal;
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
	g_running = false;
}

void	connection_inactivity_callback(UA_Client *client)
{
	t_TagInfo		*tags;
	UA_StatusCode	retval;

	g_server = 0;
	tags = (t_TagInfo *)UA_Client_getConfig(client)->clientContext;
	printf("Server disconnected. Attempting to reconnect...\n");
	UA_Client_disconnect(client);
	retval = UA_Client_connect(client, g_url);
	while (retval != UA_STATUSCODE_GOOD)
	{
		printf("Failed to reconnect.\n");
		retval = UA_Client_connect(client, g_url);
	}
}

void	connection_state_callback(UA_Client *client,
						UA_SecureChannelState chanelstate ,
						UA_SessionState sessionstate,
						UA_StatusCode connectstatus)
{
	t_TagInfo	*tags;

	if (connectstatus == UA_STATUSCODE_GOOD && sessionstate == UA_SESSIONSTATE_ACTIVATED && chanelstate == UA_SECURECHANNELSTATE_OPEN)
	{
		printf("Connection Established.\n");
		tags = (t_TagInfo *)UA_Client_getConfig(client)->clientContext;
		while (tags)
			tags = create_subscription(client, tags);
		g_server = 1;
	}
}

UA_Client	*connect_opc_server(const char *serverUrl, t_TagInfo *tags)
{
	UA_Client		*client;
	UA_StatusCode	retval;
	g_url = ft_strdup(serverUrl);


	signal(SIGINT, stop_handler);
	signal(SIGTERM, stop_handler);
	client = UA_Client_new();
	UA_ClientConfig_setDefault(UA_Client_getConfig(client));
	UA_Client_getConfig(client)->stateCallback = connection_state_callback;
	UA_Client_getConfig(client)->inactivityCallback = connection_inactivity_callback;
	UA_Client_getConfig(client)->requestedSessionTimeout = 0;
	UA_Client_getConfig(client)->connectivityCheckInterval = 10;
	UA_Client_getConfig(client)->clientContext = tags;
	retval = UA_Client_connect(client, serverUrl);
	while (g_running)
	{
		if(g_server)
		{
			if (g_server == 1)
			{
				g_server = 2;
				sleep(10);
			}
			UA_Client_run_iterate(client, 20);
		}
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