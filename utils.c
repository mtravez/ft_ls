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

int compare_time(void *x, void *y) {
	t_file *x_file;
	t_file *y_file;
	
	x_file = (t_file *) x;
	y_file = (t_file *) y;
 	char *x_time = x_file->utime;
	char *y_time = y_file->utime;
	size_t length = ft_strlen(x_time);
	if (length < ft_strlen(y_time)){
		length = ft_strlen(y_time);
	}
	int result = ft_strncmp(x_time, y_time, length);
	return result;
}

int compare_name(void *x, void *y) {
	t_file *x_file;
	t_file *y_file;
	
	x_file = (t_file *) x;
	y_file = (t_file *) y;
 	char *x_name = x_file->name;
	char *y_name = y_file->name;
	size_t length = ft_strlen(x_name);
	if (length < ft_strlen(y_name)){
		length = ft_strlen(y_name);
	}
	int result = ft_strncmp(x_name, y_name, length);
	// ft_printf("%s : %s = %i\n", x_name, y_name, result);
	return result;
}

int compare_name_reverse(void *x, void *y) {
	t_file *x_file;
	t_file *y_file;
	
	x_file = (t_file *) x;
	y_file = (t_file *) y;
 	char *x_name = x_file->name;
	char *y_name = y_file->name;
	size_t length = ft_strlen(x_name);
	if (length < ft_strlen(y_name)){
		length = ft_strlen(y_name);
	}
	return ft_strncmp(y_name, x_name, length);
}

t_list *ft_lstsort( int (*f)(void *, void *), t_list *head, t_list *tail) {
	t_list *prev = head;
	t_list *curr = head;
	t_list *pivot = head;
	
	while (curr != tail->next) {
		if (f(curr->content, pivot->content) < 0) {
			void *temp = curr->content;
			curr->content = prev->next->content;
			prev->next->content = temp;
			
			prev = prev->next;
		}
		curr = curr->next;
	}
	void *temp = pivot->content;
	pivot->content = prev->content;
	prev->content = temp;
	return (prev);
}

void quicksort_helper(t_list *head, t_list *tail) {
	if (head == NULL || head == tail) {
		return;
	}
	
	t_list *pivot = ft_lstsort(&compare_name_reverse, head, tail);
	quicksort_helper(head, pivot);
	quicksort_helper(pivot->next, tail);
	
}

t_file *new_file(char *name, char *path, char *utime) {
	t_file *new;
	
	new = malloc(sizeof(t_file));
	if (!new) {
		return NULL;
	}
	new->name = name;
	new->path = path;
	new->utime = utime;
	return new;
}

void print_files(void *file) {
	t_file *p_file;
	
	p_file = (t_file *) file;
	ft_printf(p_file->name);
	ft_printf("\n");
}

void sorting() {
	char *arr[] = {"abcde", "werty", "poiuyt", "bcdef", "bcdefg"};
	t_list *lst;
	lst = ft_lstnew(new_file(arr[0], "", ""));
	for (int i = 1; i < 5; i++) {
		ft_lstadd_back(&lst, ft_lstnew(new_file(arr[i], "", "")));
	}
	quicksort_helper(lst, ft_lstlast(lst));
	ft_lstiter(lst, &print_files);
}