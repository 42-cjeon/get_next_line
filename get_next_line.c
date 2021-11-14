/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjeon <cjeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 10:19:34 by cjeon             #+#    #+#             */
/*   Updated: 2021/11/14 15:35:20 by cjeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"

static void free_buffers(t_buffer **head)
{
	t_buffer *curr;
	t_buffer *next;
	
	curr = *head;
	while (curr != NULL)
	{
		next = (curr)->next_buffer;
		free(curr);
		curr = next;
	}
	*head = NULL;
}

static t_buffer *get_buffer(void)
{
	t_buffer *new_buffer;

	new_buffer = (t_buffer *)malloc(sizeof(t_buffer));
	if (new_buffer == NULL)
		return (NULL);
	new_buffer->cursor = 0;
	new_buffer->end = 0;
	new_buffer->next_buffer = NULL;
	return (new_buffer);
}

static ssize_t load_buffer(int fd, t_buffer *buffer)
{
	ssize_t len;

	len = read(fd, buffer->data, BUFFER_SIZE);
	if (0 < len)
		buffer->end = len;
	return (len);
}

static size_t move_buffer_head(t_buffer **head)
{
	t_buffer *next;

	next = (*head)->next_buffer;
	free(*head);
	*head = next;
	return (0);
}

static char *copy_buffer(t_buffer **head, size_t start, size_t total_len)
{
	char		*str;
	size_t		i;

	str = malloc(sizeof(char) * (total_len + 1));
	if (str == NULL)
		return (NULL);
	str[total_len] = '\0';
	i = 0;
	while(head && start <= (*head)->end)
	{
		if (i == total_len)
			break ;
		if (start == (*head)->end)
			start = move_buffer_head(head);
		else
			str[i++] = (*head)->data[start++];
	}
	return (str);
}

size_t get_total_len(t_buffer *head, int fd)
{
	ssize_t read_len;
	size_t total_len;
	t_buffer *next_buffer;

	total_len = 0;
	while(head->cursor <= head->end)
	{
		if (head->cursor == head->end)
		{
			next_buffer = get_buffer();
			if (next_buffer == NULL)
				return (0);
			read_len = load_buffer(fd, next_buffer);
			if (read_len < 1)
			{
				free(next_buffer);
				if (read_len == 0)
					return total_len;
				return  0;
			}
			head->next_buffer = next_buffer;
			head = head->next_buffer;
			continue ;
		}
		if (head->data[head->cursor] == '\n')
		{
			head->cursor++;
			break ;
		}
		head->cursor++;
		total_len++;
	}
	return (total_len + 1);
}

char *get_next_line(int fd)
{
	static t_buffer *head;
	ssize_t	len;
	size_t total_len;
	size_t start;

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
