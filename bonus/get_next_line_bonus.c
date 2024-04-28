/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timhopgood <timhopgood@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:04:39 by timhopgood        #+#    #+#             */
/*   Updated: 2024/04/27 20:02:22 by timhopgood       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#define BOOL 0
#define LENGTH 1

/*
 * Creates and appends new node with content 'content' to tail of list.
 */

static int	ft_list_append(t_list **list, char *content, int fd)
{
	t_list	*new_node;
	t_list	*tail;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return (ft_dealloc_list(&list[fd]), -1);
	new_node->string = content;
	new_node->next = NULL;
	if (list[fd] == NULL)
	{
		list[fd] = new_node;
		return (0);
	}
	tail = ft_lstlast(list[fd]);
	tail->next = new_node;
	return (0);
}

/*
 *	sets oflow node's string field to oflow string. deallocates list.
 *	sets static list head to oflow node if oflow string exists,
 *	otherwise frees.
 */

static void	ft_process_oflow(t_list **list, t_list *oflow_node,
		char *oflow_string)
{
	oflow_node->string = oflow_string;
	oflow_node->next = NULL;
	ft_dealloc_list(list);
	if (oflow_node->string[0] && oflow_node)
		*list = oflow_node;
	else
	{
		free(oflow_string);
		free(oflow_node);
	}
}

/*
 *	Creates overflow node/string to hold remainder of string, if any,
 *	after \n from last node.
 */

static int	ft_build_oflow(t_list **list)
{
	t_list	*tail;
	t_list	*oflow_node;
	char	*oflow_string;
	int		i;
	int		k;

	oflow_string = malloc(BUFFER_SIZE + 1);
	if (oflow_string == NULL)
		return (-1);
	oflow_node = malloc(sizeof(t_list));
	if (oflow_node == NULL)
		return (free(oflow_string), -1);
	tail = ft_lstlast(*list);
	i = 0;
	k = 0;
	while (tail->string[i] && tail->string[i] != '\n')
		++i;
	while (tail->string[i] && tail->string[++i])
		oflow_string[k++] = tail->string[i];
	oflow_string[k] = '\0';
	ft_process_oflow(list, oflow_node, oflow_string);
	return (0);
}

/*
 *	Continually checks if static list contains '\n' and builds list until it
 *		does. Appending each buffer to a new node in the linked list.
 */

static void	ft_populate_list(t_list **list, int fd)
{
	int		bytes_read;
	char	*buffer;

	while (ft_findnewline(list[fd], '\n', BOOL) == 0)
	{
		buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
		if (buffer == NULL)
			return (ft_dealloc_list(&list[fd]));
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			ft_dealloc_list(&list[fd]);
			return (free(buffer));
		}
		else if (bytes_read == 0)
			return (free(buffer));
		buffer[bytes_read] = '\0';
		if (ft_list_append(list, buffer, fd) == -1)
			return (free(buffer));
	}
}

/*
 *	Builds linked list with buffer obtained from fd.
 *	Determines length of line (until \n) and mallocates memory for it.
 *	Returns new line.
 *	Cleans list in preparation for next call.
 */

char	*get_next_line(int fd)
{
	static t_list	*list[4096];
	int				next_line_length;
	char			*next_line;

	if (fd < 0 || 4095 < fd)
		return (NULL);
	ft_populate_list(list, fd);
	if (list[fd] == NULL)
		return (NULL);
	next_line_length = ft_findnewline(list[fd], '\n', LENGTH);
	next_line = malloc(next_line_length + 1);
	if (next_line == NULL)
		return (ft_dealloc_list(&list[fd]), NULL);
	ft_copy_string(list[fd], next_line);
	if (ft_build_oflow(&list[fd]) == -1)
	{
		ft_dealloc_list(&list[fd]);
		return (free(next_line), NULL);
	}
	return (next_line);
}

// ! segfault in deallocate
// ! could deallocate list and buffer separately
// ! is returning -1 from functions necessary? Check

/* int	main(void)
{
	int		fd;
	char	*buffer;
	int		lines_read;

	lines_read = 1;
	fd = open("test.txt", O_RDONLY);
	while ((buffer = get_next_line(fd)))
		printf("%d->%s\n", lines_read++, buffer);
	return (0);
} */

// fd = open("test.txt", O_RDWR | O_CREAT);

// #include <stdio.h>
// #include <fcntl.h>

// int	main(void)
// {
// 	int fd;
// 	char *next_line;
// 	int count;

//     count = 0;
//     fd = open("test.txt", O_RDONLY);
//     next_line = get_next_line(fd);
//     count ++;
// 	while ()
//     printf("[%d]: %s\n", count, next_line);
//     close(fd);
// 	return (0);
// }

// int main()
// {
// 	int fd;
// 	char *line;
// 	int lines;

// 	lines = 1;
// 	fd = open("test.txt", O_RDONLY);
// 	printf("%d fd\n", fd);

// 	line = get_next_line(fd);
// 	printf("%d->%s\n", lines++, line);
// 	line = get_next_line(fd);
// 	printf("%d->%s\n", lines++, line);
// 	line = get_next_line(fd);
// 	printf("%d->%s\n", lines++, line);
// 	line = get_next_line(fd);
// 	printf("%d->%s\n", lines++, line);
// 	// line = get_next_line(fd);
// 	// printf("%d->%s\n", lines++, line);
// 	// line = get_next_line(-1);
// 	// printf("%d->%s\n", lines++, line);
// }