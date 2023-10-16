/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpcService.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:45:43 by ade-pinh          #+#    #+#             */
/*   Updated: 2023/10/16 09:37:05 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/OpcLib.h"

int	read_config(char **ip, char **file_path, char **db_path)
{
	json_t			*root;
	json_t			*ip_value;
	json_t			*file_path_value;
	json_t			*db_path_value;
	json_error_t	error;

	root = json_load_file(CONFIG_FILE, 0, &error);
	if (root)
	{
		ip_value = json_object_get(root, "ip");
		file_path_value = json_object_get(root, "filePath");
		db_path_value = json_object_get(root, "dbPath");
		if (!(json_is_string(ip_value)
				&& json_is_string(file_path_value)
				&& json_is_string(db_path_value)))
			return (0);
		*ip = ft_strdup(json_string_value(ip_value));
		*file_path = ft_strdup(json_string_value(file_path_value));
		*db_path = ft_strdup(json_string_value(db_path_value));
		json_decref(root);
	}
	return (1);
}

int	main(void)
{
	char					*dbpath;
	char					*filepath;
	char					*url;
	UA_Client				*client;
	t_TagInfo				*tags;
	t_TagInfo				*tagsptr;

	if (!read_config(&url, &filepath, &dbpath))
	{
		printf("Can't read json file");
		return (0);
	}
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
