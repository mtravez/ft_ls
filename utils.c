/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtravez <mtravez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:51:05 by mtravez           #+#    #+#             */
/*   Updated: 2025/09/29 15:31:08 by mtravez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_file *new_file(char *name, char *path, time_t mtime) {
	t_file *new;
	
	new = malloc(sizeof(t_file));
	if (!new) {
		return NULL;
	}
	new->name = name;
	new->path = path;
	new->mtime = mtime;
	return new;
}

void print_files(void *file) {
	t_file *p_file;
	
	p_file = (t_file *) file;
	ft_printf(p_file->name);
	ft_printf("\n");
}

void free_node(void *node) {
	t_file *file = (t_file *)node;
	if (file->name)
		free(file->name);
	if (file->path)
		free(file->path);
	file->name = NULL;
	file->path = NULL;
}

void free_list(t_list **head) {
	ft_lstclear(head, &free_node);
}

void print_ls_error(char *msg) {
	ft_putstr_fd("ls: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
}

void stat_error(char *name, t_list **list) {
	if (*list != NULL) {
		free_list(list);
	}
	ft_putstr_fd("ft_ls: cannot open directory '", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd("': ", STDERR_FILENO);
	perror(NULL);
	exit(1);
}