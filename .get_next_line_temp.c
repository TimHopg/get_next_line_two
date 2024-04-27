/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   .get_next_line_temp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timhopgood <timhopgood@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:04:39 by timhopgood        #+#    #+#             */
/*   Updated: 2024/04/26 23:18:31 by timhopgood       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#define BOOL 0
#define LENGTH 1

/*
 * Traverses to tail of list and returns.
 */

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*node;

	if (lst == NULL)
		return (NULL);
	node = lst;
	while (node->next != NULL)
		node = node->next;
	return (node);
}

/*
 * Creates and appends new node with content 'content' to tail of list.
 */

void	ft_list_append(t_list **list, char *content)
{
	t_list	*new_node;
	t_list	*tail;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;
	new_node->string = content;
	new_node->next = NULL;
	if (*list == NULL)
	{
		*list = new_node;
		return ;
	}
	tail = ft_lstlast(*list);
	tail->next = new_node;
	return ;
}

/*
 *	Returns position + 1 of char 'c' if found, else, 0 if mode is BOOL (0),
 *		if mode is LENGTH (1) total length of string
 */

int	ft_findnewline(t_list *list, char c, int mode)
{
	int	i;
	int	length;

	length = 0;
	while (list)
	{
		i = 0;
		while (list->string[i])
		{
			if (list->string[i] == c)
				return (++length);
			++i;
			++length;
		}
		list = list->next;
	}
	return (mode * length);
}

/*
 *	Copies string from linked list up to and including \n then terminates
 */

void	ft_copy_string(t_list *list, char *string)
{
	int	i;
	int	k;

	if (list == NULL)
		return ;
	k = 0;
	while (list)
	{
		i = 0;
		while (list->string[i])
		{
			if (list->string[i] == '\n')
			{
				string[k++] = '\n';
				string[k] = '\0';
				return ;
			}
			string[k++] = list->string[i++];
		}
		list = list->next;
	}
	string[k] = '\0';
}

/*
 *	Deallocates list and if overflow node/string exist, deallocates them.
 *	If called after a 'read_fail', skips overflow step.
 */

void	ft_dealloc_list(t_list **list)
{
	t_list	*temp_node;
	t_list	*curr_node;

	curr_node = *list;
	temp_node = NULL;
	while (curr_node)
	{
		temp_node = curr_node->next;
		free(curr_node->string);
		free(curr_node);
		curr_node = temp_node;
	}
	*list = NULL;
}

void	ft_process_oflow(t_list **list, t_list *oflow_node, char *oflow_string)
{
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

void	ft_build_oflow(t_list **list, t_list *oflow_node, char *oflow_string)
{
	t_list	*tail;
	int		i;
	int		k;

	tail = ft_lstlast(*list);
	i = 0;
	k = 0;
	while (tail->string[i] && tail->string[i] != '\n')
		++i;
	while (tail->string[i] && tail->string[++i])
		oflow_string[k++] = tail->string[i];
	oflow_string[k] = '\0';
	oflow_node->string = oflow_string;
	oflow_node->next = NULL;
}

/*
 *	Continually checks if static list contains '\n' and builds list until it
 *		does. Appending each buffer to a new node in the linked list.
 */

void	ft_populate_list(t_list **list, int fd)
{
	int		bytes_read;
	char	*buffer;

	while (ft_findnewline(*list, '\n', BOOL) == 0)
	{
		buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
		if (buffer == NULL)
			return ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			ft_dealloc_list(list);
			return (free(buffer));
		}
		else if (bytes_read == 0)
			return (free(buffer));
		buffer[bytes_read] = '\0';
		ft_list_append(list, buffer);
	}
}

/*
 *	Builds linked list with buffer obtained from fd.
 *	Determines length of line (until \n) and mallocates memory for it.
 *	Cleans list in preparation for next call.
 */

char	*get_next_line(int fd)
{
	static t_list	*list = NULL;
	int				next_line_length;
	char			*next_line;
	t_list			*oflow_node;
	char			*oflow_string;

	ft_populate_list(&list, fd);
	if (list == NULL)
		return (NULL);
	next_line_length = ft_findnewline(list, '\n', LENGTH);
	next_line = malloc(next_line_length + 1);
	if (next_line == NULL)
		return (NULL);
	ft_copy_string(list, next_line);
	oflow_string = malloc(BUFFER_SIZE + 1);
	oflow_node = malloc(sizeof(t_list));
	if (oflow_string == NULL || oflow_node == NULL)
		return (NULL);
	ft_build_oflow(&list, oflow_node, oflow_string);
	ft_dealloc_list(&list);
	ft_process_oflow(&list, oflow_node, oflow_string);
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