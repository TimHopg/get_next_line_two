/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 22:28:02 by thopgood          #+#    #+#             */
/*   Updated: 2024/05/03 11:35:40 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE -1
# endif

typedef struct s_list
{
	char			*string;
	struct s_list	*next;
}					t_list;

char				*get_next_line(int fd);
t_list				*ft_lstlast(t_list *lst);
void				ft_dealloc_list(t_list **list);
int					ft_findnewline(t_list *list, char c, int mode);
void				ft_copy_string(t_list *list, char *string);

#endif