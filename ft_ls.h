/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtravez <mtravez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:58:58 by mtravez           #+#    #+#             */
/*   Updated: 2025/10/15 15:09:49 by mtravez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <pwd.h>
# include <grp.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <time.h>
# include "libft/libft.h"

# define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef struct s_file {
	char *name;
	time_t mtime;
	char *path;
}	t_file;

typedef struct s_flags {
	bool	reverse_order;
	bool	mod_time_order;
	bool	all_files;
	bool	long_format;
	bool	display_recursive;
}	t_flags;

typedef struct s_padding {
	int	links;
	int	user;
	int	group;
	int	size;
}	t_padding;

void	sorting();
t_file	*new_file(char *name, char *path, time_t mtime);
void	print_files(void *file);
void	sort_a(void *lst);
void	sort_r(void *lst);
void	sort_t(void *lst);
void	sort_single_files(void *lst);
void	free_list(t_list **head);
t_flags	get_flags(int argc, char **argv);
void	print_flags(t_flags flags);
void	stat_error(char *name, t_list **list);

#endif