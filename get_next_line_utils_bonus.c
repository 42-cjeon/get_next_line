/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjeon <cjeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 10:19:39 by cjeon             #+#    #+#             */
/*   Updated: 2021/11/15 00:09:04 by cjeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line_bonus.h"

void	*free_buffers(t_buffer_head **hash_table, t_buffer_head *head, int fd)
{
	t_buffer		*curr;
	t_buffer		*next;
	t_buffer_head	*prev;

	curr = (head)->buffer;
	while (curr != NULL)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	if (hash_table[fd % TABLE_SIZE] == head)
		hash_table[fd % TABLE_SIZE] = head->next;
	else
	{
		prev = hash_table[fd % TABLE_SIZE];
		while (prev->next != head)
			prev = prev->next;
		prev->next = head->next;
	}
	free(head);
	return (NULL);
}

t_buffer	*get_buffer(void)
{
	t_buffer	*new_buffer;

	new_buffer = (t_buffer *)malloc(sizeof(t_buffer));
	if (new_buffer == NULL)
		return (NULL);
	new_buffer->end = 0;
	new_buffer->next = NULL;
	return (new_buffer);
}

ssize_t	load_buffer(int fd, t_buffer *buffer)
{
	ssize_t	len;

	len = read(fd, buffer->data, BUFFER_SIZE);
	if (0 < len)
		buffer->end = len;
	return (len);
}

size_t	move_next_buffer(t_buffer_head *head, t_buffer **buffer)
{
	t_buffer	*next;

	next = (*buffer)->next;
	free(*buffer);
	head->buffer = next;
	*buffer = next;
	return (0);
}

char	*copy_buffer(t_buffer_head **hash_table, t_buffer_head *head,
						size_t total_len, int fd)
{
	char		*str;
	size_t		i;
	size_t		start;
	t_buffer	*buffer;

	str = malloc(sizeof(char) * (total_len + 1));
	if (str == NULL)
		return (free_buffers(hash_table, head, fd));
	str[total_len] = '\0';
	i = 0;
	buffer = head->buffer;
	start = head->cursor;
	while (buffer && start <= buffer->end)
	{
		if (i == total_len)
			break ;
		if (start == buffer->end)
			start = move_next_buffer(head, &buffer);
		else
			str[i++] = buffer->data[start++];
	}
	head->cursor = start;
	return (str);
}
