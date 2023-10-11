/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcService.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:45:43 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/11 15:15:14 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpcLib.h"

UA_Boolean g_running = true;

static void	stop_handler(int signal)
{
	(void)signal;
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
	g_running = false;
}

int	main(void)
{
	char		*filename;
	char		*url;
	UA_Client	*client;
	t_TagInfo	*tags;

	signal(SIGINT, stop_handler);
	signal(SIGTERM, stop_handler);
	filename = ft_strdup("tags.xml");
	url = ft_strdup("opc.tcp://192.168.10.1:4840");
	tags = readxml(filename);
	if (!tags)
	{
		printf("Error Reading xml file, Tags dont found!\n");
		return (0);
	}
	client = connect_opc_server(url);
	while (tags)
	{
		create_subscription(client, tags);
		tags = tags->next;
	}
	while (g_running)
		UA_Client_run_iterate(client, 20);
	disconnect_opc_server(client);
	return (0);
}
