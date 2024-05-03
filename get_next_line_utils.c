/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 22:27:42 by thopgood          #+#    #+#             */
/*   Updated: 2024/04/28 22:27:43 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
 * Traverses to tail of list and returns tail node.
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
 *	Deallocates list and list contents
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

/*
 *	Returns position + 1 of char 'c' if found, else 0 if mode is BOOL (0),
 *		if mode is LENGTH (1), total length of string
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
