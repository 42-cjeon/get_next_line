/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cjeon <cjeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 10:19:31 by cjeon             #+#    #+#             */
/*   Updated: 2021/11/14 02:40:04 by cjeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

#include <stddef.h>

typedef enum e_error {
	ERROR=-1
}	t_error;

typedef struct s_buffer {
	size_t end;
	size_t cursor;
	unsigned char data[BUFFER_SIZE];
	struct s_buffer *next_buffer;
}	t_buffer;

char *get_next_line(int fd);

#endif