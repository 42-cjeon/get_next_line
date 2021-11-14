/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjeon <cjeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 10:19:31 by cjeon             #+#    #+#             */
/*   Updated: 2021/11/14 16:36:04 by cjeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stddef.h>
# include <unistd.h>

typedef enum e_error {
	ERROR=-1
}	t_error;

typedef struct s_buffer {
	size_t			end;
	size_t			cursor;
	unsigned char	data[BUFFER_SIZE];
	struct s_buffer	*next_buffer;
}	t_buffer;

char		*get_next_line(int fd);
char		*copy_buffer(t_buffer **head, size_t start, size_t total_len);
size_t		move_buffer_head(t_buffer **head);
ssize_t		load_buffer(int fd, t_buffer *buffer);
t_buffer	*get_buffer(void);
void		free_buffers(t_buffer **head);
ssize_t		alloc_and_read_buffer(t_buffer *head, int fd);
size_t		get_total_len(t_buffer *head, int fd);

#endif