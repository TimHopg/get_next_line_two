/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:04:31 by timhopgood        #+#    #+#             */
/*   Updated: 2024/04/28 12:55:35 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
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