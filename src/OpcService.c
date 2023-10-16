/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcService.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:45:43 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/16 09:15:57 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/OpcLib.h"

int	main(void)
{
	char					*dbpath;
	char					*filepath;
	char					*url;
	UA_Client				*client;
	t_TagInfo				*tags;
	t_TagInfo				*tagsptr;

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
	client = connect_opc_server(url, tags);
	disconnect_opc_server(client);
	close_db();
	free(filepath);
	free(url);
	while (tags)
	{
		tagsptr = tags;
		tags = tags->next;
		free(tagsptr->name);
		free(tagsptr->node);
		free(tagsptr->tag_type);
		free(tagsptr);
	}
	return (0);
}
