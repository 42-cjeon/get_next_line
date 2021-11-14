/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjeon <cjeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 10:19:34 by cjeon             #+#    #+#             */
/*   Updated: 2021/11/14 16:32:09 by cjeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"

ssize_t	alloc_and_read_buffer(t_buffer *head, int fd)
{
	t_buffer	*next_buffer;
	ssize_t		read_len;

	next_buffer = get_buffer();
	if (next_buffer == NULL)
		return (-1);
	read_len = load_buffer(fd, next_buffer);
	if (read_len < 1)
		free(next_buffer);
	else
		head->next_buffer = next_buffer;
	return (read_len);
}

size_t	get_total_len(t_buffer *head, int fd)
{
	ssize_t		read_len;
	size_t		total_len;

	total_len = 0;
	while (head->cursor <= head->end)
	{
		if (head->cursor == head->end)
		{
			read_len = alloc_and_read_buffer(head, fd);
			if (read_len == 0)
				return (total_len);
			else if (read_len == -1)
				return (0);
			head = head->next_buffer;
			continue ;
		}
		if (head->data[head->cursor++] == '\n')
			break ;
		total_len++;
	}
	return (total_len + 1);
}

char	*get_next_line(int fd)
{
	static t_buffer	*head;
	ssize_t			len;
	size_t			total_len;
	size_t			start;

	len = 1;
	if (head == NULL)
	{
		head = get_buffer();
		if (head == NULL)
			return (NULL);
		len = load_buffer(fd, head);
		if (len < 1)
		{
			free(head);
			head = NULL;
			return (NULL);
		}
	}
	start = head->cursor;
	total_len = get_total_len(head, fd);
	if (total_len)
		return (copy_buffer(&head, start, total_len));
	free_buffers(&head);
	return (NULL);
}
