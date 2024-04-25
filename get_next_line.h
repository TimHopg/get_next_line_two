/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timhopgood <timhopgood@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:04:31 by timhopgood        #+#    #+#             */
/*   Updated: 2024/04/25 11:47:27 by timhopgood       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

typedef struct s_list
{
	char			*string;
	struct s_list	*next;
}					t_list;

char				*get_next_line(int fd);
t_list				*find_last_node(t_list *list);
int					is_newline(t_list *list);
void				copy_str(t_list *list, char *str);
char				*grab_line(t_list *list);
void				append(t_list **list, char *buffer);
void				dealloc(t_list **list, t_list *clean_node, char *buf);
void				clean_list(t_list **list);
int	ft_line_length(t_list *list);
void				create_list(t_list **list, int fd);

#endif