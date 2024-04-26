/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_temp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:04:39 by timhopgood        #+#    #+#             */
/*   Updated: 2024/04/26 18:22:58 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#define BOOL 0
#define LENGTH 1

void	ft_deallocate_list(t_list **head)
{
	t_list	*curr;
	t_list	*temp;

	curr = NULL;
	temp = NULL;
	curr = *head;
	while (curr)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	*head = NULL;
}

// creates and appends new node with content 'content' to tail of list

void	ft_list_append(t_list **list, char *content)
{
	t_list	*new_node;
	t_list	*tail;

	// cursory check
	if (list == NULL || content == NULL)
		return ;
	// ! creates new node ! MALLOC
	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;
	// adds content to string field
	new_node->string = content;
	// sets next field to NULL
	new_node->next = NULL;
	// if list is empty, new node becomes list
	if (*list == NULL)
	{
		*list = new_node;
		return ;
	}
	// tail traverses list and new node is appended to end
	tail = *list;
	while (tail->next != NULL)
		tail = tail->next;
	tail->next = new_node;
	return ;
}

/*
returns position + 1 of char 'c' if found, else, 0 if mode is BOOL (0),
	if mode is LENGTH (1) total length of string
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

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (*s++)
		len++;
	return (len);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize)
{
	const char	*src_ptr;

	src_ptr = src;
	if (dstsize > 0)
	{
		while (*src_ptr && dstsize-- > 1)
			*dest++ = *src_ptr++;
		*dest = '\0';
	}
	return (ft_strlen(src));
}

// void	ft_copy_string(t_list *list, char *target_string, int target_size)
// {
// 	t_list	*current;
// 	int		copied_size;

// 	current = list;
// 	copied_size = 0;
// 	while (target_size > 0)
// 	{
// 		if (BUFFER_SIZE < target_size)
// 		{
// 			printf("%d<-target size\n", target_size);
// 			printf("%s\n", target_string);

// 			ft_strlcpy(target_string + copied_size, current->string, BUFFER_SIZE + 1);
// 			target_size -= BUFFER_SIZE;
// 			copied_size += BUFFER_SIZE;
// 			current = current->next;
// 		}
// 		else
// 		{
// 			ft_strlcpy(target_string + copied_size, current->string, target_size + 1);
// 			printf("%s\n", target_string);
// 			printf("breaking\n");
// 			break ;
// 		}
// 	}
// }

// void	ft_copy_string(t_list *list, char *target_string, int target_size)
// {
// 	t_list	*current;
// 	int		copied_size;
// 	int		current_length;
// 	int		to_copy;

// 	current = list;
// 	// tracks total chars already copied
// 	copied_size = 0;
// 	// while list and still char left to copy
// 	printf("%d<-target size\n", target_size);
// 	while (current && copied_size < target_size)
// 	{
// 		current_length = ft_strlen(current->string);
// 		if (copied_size + current_length <= target_size)
// 			to_copy = current_length;
// 		else
// 			to_copy = target_size - copied_size;
// 		ft_strlcpy(target_string + copied_size, current->string, to_copy + 1);
// 		copied_size += to_copy;
// 		current = current->next;
// 	}
// }

// void copyStringsToTarget(Node *head, char *target, size_t targetSize) {
//     Node *current = head;
//     size_t copiedSize = 0;

//     // Iterate through the linked list nodes
//     while (current != NULL && copiedSize < targetSize) {
//         size_t len = strlen(current->data);

//         // Determine how many characters to copy
//         size_t toCopy;
//         if (copiedSize + len <= targetSize) {
//             toCopy = len;
//         } else {
//             toCopy = targetSize - copiedSize;
//         }

//        
	// Copy characters from the current node's string to the target string
//         strncpy(target + copiedSize, current->data, toCopy);

//         copiedSize += toCopy;
//         current = current->next;
//     }

void	ft_copy_string(t_list *list, char *string)
{
	int	i;
	int	k;

	// cursory check
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
void	ft_dealloc_all(t_list **list, t_list *overflow_node, char *overflow_string, int read_fail)
{
	t_list	*temp_node;
	t_list	*curr_node;

	curr_node = *list;
	temp_node = NULL;
	// frees current list
	while (curr_node)
	{
		temp_node = curr_node->next;
		free(curr_node->string);
		free(curr_node);
		curr_node = temp_node;
	}
	// sets list pointer to null
	*list = NULL;
	// if overflow node and string exist, overflow node becomes new list
	if (overflow_node->string[0] && overflow_node && read_fail != -1)
	{
		// printf("dealloc fail\n");
		*list = overflow_node;
	}
	// other wise frees both overflow_node and overflow_string 
	else
	{
		// printf("dealloc check\n");
		free(overflow_string);
		free(overflow_node);
	}
}

// void	ft_dealloc_all(t_list **list, t_list *overflow_node, char *buffer)
// {
// 	t_list	*temp_node;
// 	t_list	*curr_node;

// 	curr_node = *list;
// 	temp_node = NULL;
// 	while (curr_node)
// 	{
// 		temp_node = curr_node->next;
// 		free(curr_node->string);
// 		free(curr_node);
// 		curr_node = temp_node;
// 	}
// 	*list = NULL;
	
// 	if (overflow_node->string[0] && overflow_node)
// 	{
// 		*list = overflow_node;
// 	}
// 	else
// 	{
// 		free(buffer);
// 		free(overflow_node);
// 	}
// }

void	ft_trim_list(t_list **list, int read_fail)
{
	t_list	*tail;
	t_list	*overflow_node;
	int		i;
	int		k;
	char	*overflow_string;

	if (*list == NULL)
		return ;
	// ! this is the memory LEAK
	overflow_string = malloc(BUFFER_SIZE + 1);
	// creates overflow_node to hold text after \n
	overflow_node = malloc(sizeof(t_list));
	if (overflow_string == NULL || overflow_node == NULL)
		return ;
	tail = *list;
	while (tail->next != NULL)
		tail = tail->next;
	i = 0;
	k = 0;
	// scans to end of previously printed line
	while (tail->string[i] && tail->string[i] != '\n')
		++i;
	// captures char until \0
	while (tail->string[i] && tail->string[++i])
		overflow_string[k++] = tail->string[i];
	// terminates
	overflow_string[k] = '\0';
	// sets string field of overflow_node 
	overflow_node->string = overflow_string;
	overflow_node->next = NULL;
	return (ft_dealloc_all(list, overflow_node, overflow_string, read_fail));
}

/*	Continually checks if static list contains '\n' and builds list until it
 *	does.
 */

int	ft_populate_list(t_list **list, int fd)
{
	int		bytes_read;
	char	*buffer;

	// loops through list to check for \n and continues while it is not found
	while (ft_findnewline(*list, '\n', BOOL) == 0)
	{
		// allocates buffer + 1 (for \0) size chunk of memory
		buffer = malloc(sizeof(char) * BUFFER_SIZE + 1);
		if (buffer == NULL)
			// if it returns, does the function properly handle that
			return (-1);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		// if read fails
		// ! deal with a failure on the > 1st call
		if (bytes_read == -1)
		{
			ft_deallocate_list(list);
			free(buffer);
			return (-1);
		}
		else if (bytes_read == 0)
			return ((free(buffer)), 0);
		buffer[bytes_read] = '\0';
		ft_list_append(list, buffer);
	}
	return (0);
}

char	*get_next_line(int fd)
{
	static t_list	*list = NULL;
	int				next_line_length;
	char			*next_line;
	int				read_fail;

	read_fail = ft_populate_list(&list, fd);
	if (list == NULL)
		return (NULL);
	next_line_length = ft_findnewline(list, '\n', LENGTH);
	next_line = malloc(next_line_length + 1);
	if (next_line == NULL)
		return (NULL);
	ft_copy_string(list, next_line);
	ft_trim_list(&list, read_fail);
	return (next_line);
}
