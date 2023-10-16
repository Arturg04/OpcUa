/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:31:25 by Arturg04          #+#    #+#             */
/*   Updated: 2023/10/16 11:56:33 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/OpcLib.h"

t_TagInfo	*get_array_json(json_t *json_tag_array)
{
	size_t		i;
	size_t		size;
	json_t		*json_tag;
	t_TagInfo	*tags;
	char		*node;

	i = -1;
	size = json_array_size(json_tag_array);
	tags = (t_TagInfo *)malloc(sizeof(t_TagInfo));
	if (!tags)
		return (NULL);
	tags->prev = (NULL);
	while (++i < size)
	{
		json_tag = json_array_get(json_tag_array, i);
		tags->name = ft_strdup(json_string_value(json_object_get(json_tag, "Name")));
		tags->tag_type = ft_strdup(json_string_value(json_object_get(json_tag, "Type")));
		node = (char *)json_string_value(json_object_get(json_tag, "Node"));
		tags->node = malloc(sizeof(t_node));
		if (!tags->node)
			continue ;
		node = ft_strnstr(node, "=", ft_strlen(node));
		tags->node->index = ft_atoi(++node);
		node = ft_strnstr(node, "=", ft_strlen(node));
		tags->node->identifier = ft_atoi(++node);
		tags->next = (t_TagInfo *)malloc(sizeof(t_TagInfo));
		tags->next->prev = tags;
		tags = tags->next;
	}
	if (!tags)
		return (NULL);
	tags = tags->prev;
	tags->next = NULL;
	while (tags->prev)
		tags = tags->prev;
	printf("End Read File\n");
	return (tags);
}

t_TagInfo	*read_json(char *filepath)
{
	json_t			*json_root;
	json_t			*json_tag_array;
	t_TagInfo		*tags;
	json_error_t	error;

	printf("%s\n", filepath);
	json_root = json_load_file(filepath, 0, &error);
	if (json_root == NULL)
	{
		fprintf(stderr, "Error parsing JSON: %s\n", error.text);
		return (NULL);
	}
	json_tag_array = json_object_get(json_root, "Tags");
	if (!json_is_array(json_tag_array))
	{
		printf("Don't exist tags in: %s", filepath);
		return (NULL);
	}
	tags = get_array_json(json_tag_array);
	json_decref(json_root);
	return (tags);
}
