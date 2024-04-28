/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timhopgood <timhopgood@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:04:39 by timhopgood        #+#    #+#             */
/*   Updated: 2024/04/28 22:05:57 by timhopgood       ###   ########.fr       */
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

	if (fd < 0 || 4095 < fd || BUFFER_SIZE <= 0)
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

// # include <fcntl.h> // open/write
// # include <sys/stat.h> // USR permissions
// # include <stdio.h> // printf

// /* ulimit -a -H: hard limits of system processes */

// int	main(void)
// {
// 	int fd1;
// 	int fd2;
// 	int fd3;
// 	char	*line;

// 	fd1 = open("bonus1.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
// 	write(fd1, "AAAAAAAAAA\n", 11);
// 	write(fd1, "BBBBBBBBBB\n", 11);
// 	write(fd1, "CCCCCCCCCC\n", 11);
// 	write(fd1, "DDDDDDDDDD", 10);
// 	close(fd1);

// 	fd2 = open("bonus2.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
// 	write(fd2, "EEEEEEEEEE\n", 11);
// 	write(fd2, "FFFFFFFFFF\n", 11);
// 	write(fd2, "GGGGGGGGGG\n", 11);
// 	write(fd2, "HHHHHHHHHH", 10);
// 	close(fd2);

// 	fd3 = open("bonus3.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
// 	write(fd3, "IIIIIIIIII\n", 11);
// 	write(fd3, "JJJJJJJJJJ\n", 11);
// 	write(fd3, "KKKKKKKKKK\n", 11);
// 	write(fd3, "LLLLLLLLLL", 10);
// 	close(fd3);

// 	int fd1_lines = 1;
// 	int fd2_lines = 1;
// 	int fd3_lines = 1;

// 	printf("MULTIPLE FD TESTS\n");

// 	fd1 = open("bonus1.txt", O_RDONLY);
// 	fd2 = open("bonus2.txt", O_RDONLY);
// 	fd3 = open("bonus3.txt", O_RDONLY);

// 	line = get_next_line(fd1);
// 	printf("[Line %d][File 1] %s⤶", fd1_lines++, line);
// 	free(line);

// 	line = get_next_line(fd2);
// 	printf("[Line %d][File 2] %s⤶", fd2_lines++, line);
// 	free(line);
	
// 	line = get_next_line(fd3);
// 	printf("[Line %d][File 3] %s⤶", fd3_lines++, line);
// 	free(line);

// 	line = get_next_line(fd1);
// 	printf("[Line %d][File 1] %s⤶", fd1_lines++, line);
// 	free(line);

// 	line = get_next_line(fd2);
// 	printf("[Line %d][File 2] %s⤶", fd2_lines++, line);
// 	free(line);

// 	line = get_next_line(fd3);
// 	printf("[Line %d][File 3] %s⤶", fd3_lines++, line);
// 	free(line);

// 	line = get_next_line(fd1);
// 	printf("[Line %d][File 1] %s⤶", fd1_lines++, line);
// 	free(line);

// 	line = get_next_line(fd2);
// 	printf("[Line %d][File 2] %s⤶", fd2_lines++, line);
// 	free(line);

// 	line = get_next_line(fd3);
// 	printf("[Line %d][File 3] %s⤶", fd3_lines++, line);
// 	free(line);

// 	line = get_next_line(fd1);
// 	printf("[Line %d][File 1] %s⤶", fd1_lines++, line);
// 	free(line);
// 	printf("\n");

// 	line = get_next_line(fd2);
// 	printf("[Line %d][File 2] %s⤶", fd2_lines++, line);
// 	free(line);
// 	printf("\n");
	
// 	line = get_next_line(fd3);
// 	printf("[Line %d][File 3] %s⤶", fd3_lines++, line);
// 	free(line);
// 	printf("\n\n");

// 	close(fd1);
// 	close(fd2);
// 	close(fd3);

// 	fd1_lines = 1;
// 	fd2_lines = 1;
// 	fd3_lines = 1;

// 	printf("MULTIPLE FD WITH ERROR\n");

// 	fd1 = open("bonus1.txt", O_RDONLY);
// 	fd2 = open("bonus2.txt", O_RDONLY);
// 	fd3 = open("bonus3.txt", O_RDONLY);

// 	line = get_next_line(fd1);
// 	printf("[Line %d][File 1] %s⤶", fd1_lines++, line);
// 	free(line);
// 	line = get_next_line(fd2);
// 	printf("[Line %d][File 2] %s⤶", fd2_lines++, line);
// 	free(line);
// 	line = get_next_line(fd3);
// 	printf("[Line %d][File 3] %s⤶", fd3_lines++, line);
// 	free(line);
// 	line = get_next_line(fd1);
// 	printf("[Line %d][File 1] %s⤶", fd1_lines++, line);
// 	free(line);
// 	line = get_next_line(fd2);
// 	printf("[Line %d][File 2] %s⤶", fd2_lines++, line);
// 	free(line);
// 	line = get_next_line(fd3);
// 	printf("[Line %d][File 3] %s⤶", fd3_lines++, line);
// 	free(line);
// 	line = get_next_line(fd1);
// 	printf("[Line %d][File 1] %s⤶", fd1_lines++, line);
// 	free(line);
// 	line = get_next_line(-1);
// 	printf("NON-EXISTING FD\n");
// 	printf("[Line %d][File 2] %s⤶", fd2_lines++, line);
// 	printf("\n");
// 	line = get_next_line(fd3);
// 	printf("[Line %d][File 3] %s⤶", fd3_lines++, line);
// 	free(line);
// 	line = get_next_line(fd1);
// 	printf("[Line %d][File 1] %s⤶", fd1_lines++, line);
// 	free(line);
// 	printf("\n");
// 	line = get_next_line(fd2);
// 	printf("[Line %d][File 2] %s⤶", fd2_lines++, line);
// 	free(line);
// 	line = get_next_line(fd3);
// 	printf("[Line %d][File 3] %s⤶", fd3_lines++, line);
// 	free(line);
// 	printf("\n");

// 	close(fd1);
// 	close(fd2);
// 	close(fd3);
// }
