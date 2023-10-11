/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcComm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:43:20 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/11 14:24:15 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpcLib.h"

UA_Client	*ConnectToOpcServer(const char *serverUrl)
{
	UA_Client *client = UA_Client_new();
	UA_ClientConfig_setDefault(UA_Client_getConfig(client));
	UA_StatusCode retval = UA_Client_connect(client, serverUrl);
	if(retval != UA_STATUSCODE_GOOD) {
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"Failed to connect to the server. Error code %s", UA_StatusCode_name(retval));
		return (NULL);
}
	if(retval != UA_STATUSCODE_GOOD) {
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"Failed to connect to the server. Error code %s", UA_StatusCode_name(retval));
		return (NULL);
	}
	printf("Connected to the server!\n");
	return (client);
}

void	DisconnectFromOpcServer(UA_Client *client)
{
	if (client != NULL)
	{
		UA_Client_disconnect(client);
		UA_Client_delete(client);
		printf("Disconnected from the server.\n");
	}
}