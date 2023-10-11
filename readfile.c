/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-pinh <artur.13.goncalves@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:31:25 by Arturg04          #+#    #+#             */
/*   Updated: 2023/10/11 15:10:54 by ade-pinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpcLib.h"

char	*ft_jointfree(char *str, char *buff)
{
	char	*temp;

	temp = ft_strjoin(str, buff);
	free(str);
	return (temp);
}

char	*read_file(int fd, char	*buffer)
{
	char	*buf;
	int		bytes;

	if (!buffer)
		buffer = ft_calloc(1, 1);
	buf = ft_calloc(1, BUFFER_SIZE + 1);
	bytes = 1;
	while (!ft_strchr(buffer, '\n') && bytes > 0)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == 0 || bytes < 0)
			break ;
		buffer = ft_jointfree(buffer, buf);
		free(buf);
		buf = ft_calloc(1, BUFFER_SIZE + 1);
	}
	free(buf);
	if (bytes < 0 || !*buffer)
	{
		free(buffer);
		return (NULL);
	}
	return (buffer);
}

char	*next_buffer(char	*buffer)
{
	char	*temp;
	int		i;

	i = 0;
	if (!*buffer)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		temp = ft_strjoin(buffer + i, "");
		free(buffer);
		return (temp);
	}
	temp = ft_strjoin(buffer + i + 1, "");
	free(buffer);
	return (temp);
}

char	*next_line(char	*buffer)
{
	int		i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
		return (ft_substr(buffer, 0, i));
	return (ft_substr(buffer, 0, i + 1));
}

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FILES];
	char		*line;

	if (fd < 0 && BUFFER_SIZE < 0)
		return (NULL);
	buffer[fd] = read_file(fd, buffer[fd]);
	if (!buffer[fd])
		return (NULL);
	line = next_line(buffer[fd]);
	buffer[fd] = next_buffer(buffer[fd]);
	return (line);
}

t_TagInfo	*readxml(char *filepath)
{
	int			file;
	char		*line;
	char		*subline;
	t_TagInfo	*tags;

	file = open(filepath, O_RDONLY);
	if (file < 0)
		return (NULL);
	tags = (t_TagInfo *)malloc(sizeof(t_TagInfo));
	if (!tags)
		return (NULL);
	tags->prev = (NULL);
	line = get_next_line(file);
	while (line)
	{
		if (ft_strnstr(line, "<Tags>", ft_strlen(line)))
		{
			while (!ft_strnstr(line, "</Tags>", ft_strlen(line)))
			{
				free(line);
				line = get_next_line(file);
				if (ft_strnstr(line, "<Name>", ft_strlen(line)))
				{
					subline = ft_strnstr(line, "<Name>", ft_strlen(line));
					ft_strnstr(line, "</Name>", ft_strlen(line))[0] = 0;
					tags->name = ft_substr(subline, ft_strlen("<Name>"), ft_strlen(subline));
				}
				else if (ft_strnstr(line, "<Node>", ft_strlen(line)))
				{
					subline = ft_strnstr(line, "<Node>", ft_strlen(line));
					ft_strnstr(line, "</Node>", ft_strlen(line))[0] = 0;
					tags->node = malloc(sizeof(t_node));
					if (!tags->node)
						continue ;
					subline = ft_strnstr(subline, "=", ft_strlen(subline));
					tags->node->index =ft_atoi(++subline);
					subline = ft_strnstr(subline, "=", ft_strlen(subline));
					tags->node->identifier =ft_atoi(++subline);
				}
				else if (ft_strnstr(line, "<Type>", ft_strlen(line)))
				{
					subline = ft_strnstr(line, "<Type>", ft_strlen(line));
					ft_strnstr(line, "</Type>", ft_strlen(line))[0] = 0;
					tags->tag_type = ft_substr(subline, ft_strlen("<Type>"), ft_strlen(subline));
				}
			}
			tags->next = (t_TagInfo *)malloc(sizeof(t_TagInfo));
			tags->next->prev = tags;
			tags = tags->next;
		}
		free(line);
		line = get_next_line(file);
	}
	if (line)
		free(line);
	close(file);
	if (!tags)
		return (NULL);
	tags = tags->prev;
	tags->next = NULL;
	while (tags->prev)
		tags = tags->prev;
	return (tags);
}
