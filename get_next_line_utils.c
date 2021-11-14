/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjeon <cjeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 10:19:39 by cjeon             #+#    #+#             */
/*   Updated: 2021/11/14 16:35:46 by cjeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"

void	free_buffers(t_buffer **head)
{
	t_buffer	*curr;
	t_buffer	*next;

	curr = *head;
	while (curr != NULL)
	{
		next = (curr)->next_buffer;
		free(curr);
		curr = next;
	}
	*head = NULL;
}

t_buffer	*get_buffer(void)
{
	t_buffer	*new_buffer;

	new_buffer = (t_buffer *)malloc(sizeof(t_buffer));
	if (new_buffer == NULL)
		return (NULL);
	new_buffer->cursor = 0;
	new_buffer->end = 0;
	new_buffer->next_buffer = NULL;
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

size_t	move_buffer_head(t_buffer **head)
{
	t_buffer	*next;

	next = (*head)->next_buffer;
	free(*head);
	*head = next;
	return (0);
}

char	*copy_buffer(t_buffer **head, size_t start, size_t total_len)
{
	char		*str;
	size_t		i;

	str = malloc(sizeof(char) * (total_len + 1));
	if (str == NULL)
		return (NULL);
	str[total_len] = '\0';
	i = 0;
	while (head && start <= (*head)->end)
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
