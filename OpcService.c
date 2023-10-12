/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcService.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:45:43 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/12 11:58:54 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/OpcLib.h"

UA_Boolean g_running = true;

static void	stop_handler(int signal)
{
	(void)signal;
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
	g_running = false;
}

int	main(void)
{
	char		*dbpath;
	char		*filepath;
	char		*url;
	UA_Client	*client;
	t_TagInfo	*tags;
	t_TagInfo	*tagsptr;

	signal(SIGINT, stop_handler);
	signal(SIGTERM, stop_handler);
	dbpath = ft_strdup("IEM.db");
	filepath = ft_strdup("tags.xml");
	url = ft_strdup("opc.tcp://192.168.10.1:4840");
	tags = readxml(filepath);
	if (!tags)
	{
		printf("Error Reading xml file, Tags dont found!\n");
		return (0);
	}
	if (open_db(dbpath) != 1)
		return (0);
	if (create_table() != 1)
		return (0);
	tagsptr = tags;
	while (tagsptr)
	{
		if (insert_data(tagsptr, 0) != 1)
			return (0);
		tagsptr = tagsptr->next;
	}
	client = connect_opc_server(url);
	tagsptr = tags;
	while (tagsptr)
	{
		create_subscription(client, tagsptr);
		tagsptr = tagsptr->next;
	}
	while (g_running)
		UA_Client_run_iterate(client, 20);
	disconnect_opc_server(client);
	close_db();
	free(filepath);
	free(url);
	return (0);
}
