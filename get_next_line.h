/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timhopgood <timhopgood@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:04:31 by timhopgood        #+#    #+#             */
/*   Updated: 2024/04/28 21:59:00 by timhopgood       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 0
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