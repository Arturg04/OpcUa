/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcService.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:45:43 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/11 14:36:40 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpcLib.h"

UA_Boolean running = true;

static void stopHandler(int signal)
{
	(void)signal;
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
	running = false;
}

int	main(void)
{
	signal(SIGINT, stopHandler);
	signal(SIGTERM, stopHandler);

	char		*filename = "tags.xml";
	char		*url = "opc.tcp://192.168.10.1:4840";
	UA_Client	*client;
	t_TagInfo	*Tags;
	t_TagInfo	*Tags_ptr;


	Tags = readxml(filename);
	if (!Tags)
	{
		printf("Error Reading xml file, Tags dont found!\n");
		return (0);
	}
	client = ConnectToOpcServer(url);
	Tags_ptr = Tags;
	while (Tags_ptr)
	{
		CreateSubscription(client, Tags_ptr);
		Tags_ptr = Tags_ptr->next;
	}
	while(running)
		UA_Client_run_iterate(client, 100);
	Tags_ptr = Tags;
	DisconnectFromOpcServer(client);
	return (0);
}