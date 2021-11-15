/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjeon <cjeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 10:19:34 by cjeon             #+#    #+#             */
/*   Updated: 2021/11/16 01:18:06 by cjeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line_bonus.h"

ssize_t	alloc_and_read_buffer(t_buffer *buffer, int fd)
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
		buffer->next = next_buffer;
	return (read_len);
}

size_t	get_total_len(t_buffer_head **hash_table, t_buffer_head *head,
						size_t start, int fd)
{
	t_buffer	*buffer;
	ssize_t		read_len;
	size_t		total_len;

	total_len = 0;
	buffer = head->buffer;
	while (start <= buffer->end)
	{
		if (start == buffer->end)
		{
			start = 0;
			read_len = alloc_and_read_buffer(buffer, fd);
			if (read_len == 0)
				break ;
			else if (read_len == -1)
				return ((size_t)free_buffers(hash_table, head, fd));
			buffer = buffer->next;
			continue ;
		}
		total_len++;
		if (buffer->data[start++] == '\n')
			break ;
	}
	return (total_len);
}

t_buffer_head	*get_buffer_head(int fd)
{
	t_buffer_head	*head;

	head = (t_buffer_head *)malloc(sizeof(t_buffer_head));
	if (head == NULL)
		return (NULL);
	head->buffer = get_buffer();
	if (head->buffer == NULL)
	{
		free(head);
		return (NULL);
	}
	head->next = NULL;
	head->cursor = 0;
	head->fd = fd;
	return (head);
}

t_buffer_head	*search_buffer_head(t_buffer_head **hash_table, int fd)
{
	t_buffer_head	*curr;
	t_buffer_head	*prev;

	curr = hash_table[fd % TABLE_SIZE];
	prev = NULL;
	while (curr)
	{
		if (curr->fd == fd)
			return (curr);
		prev = curr;
		curr = curr->next;
	}
	curr = get_buffer_head(fd);
	if (curr == NULL)
		return (NULL);
	if (prev == NULL)
		hash_table[fd % TABLE_SIZE] = curr;
	else
		prev->next = curr;
	return (curr);
}

char	*get_next_line(int fd)
{
	static t_buffer_head	*hash_table[TABLE_SIZE];
	t_buffer_head			*head;
	ssize_t					read_len;
	size_t					total_len;

	if (fd < 0)
		return (NULL);
	head = search_buffer_head(hash_table, fd);
	if (head == NULL)
		return (NULL);
	if (head->buffer->end == 0)
	{
		read_len = load_buffer(fd, head->buffer);
		if (read_len < 1)
			return (free_buffers(hash_table, head, fd));
	}
	total_len = get_total_len(hash_table, head, head->cursor, fd);
	if (total_len)
		return (copy_buffer(hash_table, head, total_len, fd));
	return (free_buffers(hash_table, head, fd));
}
