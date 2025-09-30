/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtravez <mtravez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:58:58 by mtravez           #+#    #+#             */
/*   Updated: 2025/09/29 14:44:53 by mtravez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <time.h>
# include "libft/libft.h"

typedef struct s_file {
	char *name;
	char *utime;
	char *path;
}	t_file;

void	sorting();
t_file	*new_file(char *name, char *path, char *utime);
void	print_files(void *file);

#endif