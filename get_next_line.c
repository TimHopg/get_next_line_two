/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timhopgood <timhopgood@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:04:39 by timhopgood        #+#    #+#             */
/*   Updated: 2024/04/28 01:55:50 by timhopgood       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#define BOOL 0
#define LENGTH 1

/*
 * Creates and appends new node with content 'content' to tail of list.
 */

static int	ft_list_append(t_list **list, char *content)
{
	t_list	*new_node;
	t_list	*tail;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return (ft_dealloc_list(list), -1);
	new_node->string = content;
	new_node->next = NULL;
	if (*list == NULL)
	{
		*list = new_node;
		return (0);
	}
	tail = ft_lstlast(*list);
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

	while (ft_findnewline(*list, '\n', BOOL) == 0)
	{
		buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
		if (buffer == NULL)
			return (ft_dealloc_list(list));
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			ft_dealloc_list(list);
			return (free(buffer));
		}
		else if (bytes_read == 0)
			return (free(buffer));
		buffer[bytes_read] = '\0';
		if (ft_list_append(list, buffer) == -1)
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
	static t_list	*list = NULL;
	int				next_line_length;
	char			*next_line;

	ft_populate_list(&list, fd);
	if (list == NULL)
		return (NULL);
	next_line_length = ft_findnewline(list, '\n', LENGTH);
	next_line = malloc(next_line_length + 1);
	if (next_line == NULL)
		return (ft_dealloc_list(&list), NULL);
	ft_copy_string(list, next_line);
	if (ft_build_oflow(&list) == -1)
	{
		ft_dealloc_list(&list);
		return (free(next_line), NULL);
	}
	return (next_line);
}

int	main(void)
{
	int		fd;
	char	*line;
	int		lines_read;

	lines_read = 1;
	fd = open("test.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	write(fd, "AAAAAAAAAA\n", 11);
	write(fd, "BBBBBBBBBB\n", 11);
	write(fd, "CCCCCCCCCC\n", 11);
	write(fd, "DDDDDDDDDD", 10);
	close(fd);
	
	fd = open("test.txt", O_RDONLY);
	while ((line = get_next_line(fd)))
		printf("%d->%s\n", lines_read++, line);
    close(fd);
	
	return (0);
}

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

// 	line = get_next_line(fd);
// 	printf("%d->%s\n", lines++, line);
// 	line = get_next_line(fd);
// 	printf("%d->%s\n", lines++, line);
// 	line = get_next_line(fd);
// 	printf("%d->%s\n", lines++, line);
// 	line = get_next_line(fd);
// 	printf("%d->%s\n", lines++, line);
// 	line = get_next_line(fd);
// 	printf("%d->%s\n", lines++, line);
// 	// line = get_next_line(-1);
// 	// printf("%d->%s\n", lines++, line);
// }