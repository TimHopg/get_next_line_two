/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timhopgood <timhopgood@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:04:39 by timhopgood        #+#    #+#             */
/*   Updated: 2024/04/25 13:26:21 by timhopgood       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#define BOOL 0
#define LENGTH 1

// finds and returns last node in list

t_list	*ft_lstlast(t_list *list)
{
	t_list	*node;

	if (list == NULL)
		return (NULL);
	node = list;
	while (node->next != NULL)
		node = node->next;
	return (node);
}

// creates and appends new node with content 'content' to tail of list

void	ft_list_append(t_list **list, void *content)
{
	t_list	*new_node;
	t_list	*tail;

	if (list == NULL || content == NULL)
		return ;
	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;
	tail = ft_lstlast(*list);
	if (tail == NULL)
		*list = new_node;
	else
		tail->next = new_node;
	new_node->string = content;
	new_node->next = NULL;
}

// returns position + 1 of '\n' if found, else 0

int	ft_newline_position(t_list *list, int mode)
{
	int	i;
	int	length;

	length = 0;
	while (list)
	{
		i = 0;
		while (list->string[i] /* && i < BUFFER_SIZE */)
		{
			if (list->string[i] == '\n')
				return (++length);
			++i;
			++length;
		}
		list = list->next;
	}
	return (mode * length);
}

// builds string from list nodes

char	*ft_get_line(t_list *list)
{
	int		str_len;
	char	*next_str;

	if (list == NULL)
		return (NULL);
	str_len = ft_newline_position(list, LENGTH);
	// printf("\n%d\n ll\n", str_len);
	// str_len = ft_newline_position(list);
	// printf("\n%d\n np\n", str_len);
	next_str = malloc(str_len + 1);
	if (NULL == next_str)
		return (NULL);
	copy_str(list, next_str);
	return (next_str);
}

// gets line string from list nodes

char	*ft_get_line_temp(t_list *list)
{
	int		str_len;
	char	*next_str;

	if (list == NULL)
		return (NULL);
	str_len = ft_newline_position(list, LENGTH);
	next_str = malloc(str_len + 1);
	if (NULL == next_str)
		return (NULL);
	copy_str(list, next_str);
	return (next_str);
}

void	copy_str(t_list *list, char *str)
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
				str[k++] = '\n';
				str[k] = '\0';
				return ;
			}
			str[k++] = list->string[i++];
		}
		list = list->next;
	}
	str[k] = '\0';
}

void	copy_str_temp(t_list *list, char *str)
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
				str[k++] = '\n';
				str[k] = '\0';
				return ;
			}
			str[k++] = list->string[i++];
		}
		list = list->next;
	}
	str[k] = '\0';
}

void	ft_deallocate_all(t_list **list, t_list *clean_node, char *buf)
{
	t_list	*temp_node;

	if (*list == NULL)
		return ;
	while (*list)
	{
		temp_node = (*list)->next;
		free((*list)->string);
		free(*list);
		*list = temp_node;
	}
	*list = NULL;
	if (clean_node->string[0])
		*list = clean_node;
	else
	{
		free(buf);
		free(clean_node);
	}
}

void	ft_trim_list(t_list **list)
{
	t_list	*last_node;
	t_list	*clean_node;
	int		i;
	int		k;
	char	*buf;

	buf = malloc(BUFFER_SIZE + 1);
	clean_node = malloc(sizeof(t_list));
	if (NULL == buf || NULL == clean_node)
		return ;
	last_node = ft_lstlast(*list);
	i = 0;
	k = 0;
	while (last_node->string[i] && last_node->string[i] != '\n')
		++i;
	while (last_node->string[i] && last_node->string[++i])
		buf[k++] = last_node->string[i];
	buf[k] = '\0';
	clean_node->string = buf;
	clean_node->next = NULL;
	ft_deallocate_all(list, clean_node, buf);
}

// determines the length of the string until newline character is encountered

// int	ft_line_length(t_list *list)
// {
// 	int	i;
// 	int	length;

// 	if (list == NULL)
// 		return (0);
// 	length = 0;
// 	while (list)
// 	{
// 		i = 0;
// 		while (list->string[i] /* && i < BUFFER_SIZE */)
// 		{
// 			if (list->string[i] == '\n')
// 				return (++length);
// 			++i;
// 			++length;
// 		}
// 		list = list->next;
// 	}
// 	return (length);
// }

void	ft_populate_list(t_list **list, int fd)
{
	int		bytes_read;
	char	*buffer;

	while (ft_newline_position(*list, BOOL) == 0)
	{
		buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
		if (buffer == NULL)
			return ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == 0)
			return (free(buffer));
		buffer[bytes_read] = '\0';
		ft_list_append(list, buffer);
	}
}

char	*get_next_line(int fd)
{
	static t_list	*list = NULL;
	char			*next_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (read(fd, &next_line, 0) < 0)
		return (ft_deallocate_all(&list, NULL, NULL), NULL);
	ft_populate_list(&list, fd);
	if (list == NULL)
		return (NULL);
	next_line = ft_get_line(list);
	ft_trim_list(&list);
	return (next_line);
}

// ! segfault in deallocate
// ! could deallocate list and buffer separately

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