/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtravez <mtravez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:51:05 by mtravez           #+#    #+#             */
/*   Updated: 2025/09/29 15:31:08 by mtravez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int compare_alphabetical(void *x, void *y) {
    char *x_name = (char *) x;
    char *y_name = (char *) y;

    return (ft_strcmp(x_name, y_name));
}

int compare_time(void *x, void *y) {
	t_file *x_file;
	t_file *y_file;
	
	x_file = (t_file *) x;
	y_file = (t_file *) y;
 	if (x_file->mtime > y_file->mtime)
        return -1;
    else if (x_file->mtime < y_file->mtime)
        return 1;
    return 0;
}

int compare_name(void *x, void *y) {
	t_file *x_file;
	t_file *y_file;
	
	x_file = (t_file *) x;
	y_file = (t_file *) y;
    return ft_strcmp(x_file->name, y_file->name);
}

int compare_name_reverse(void *x, void *y) {
	t_file *x_file;
	t_file *y_file;
	
	x_file = (t_file *) x;
	y_file = (t_file *) y;
 	char *x_name = x_file->name;
	char *y_name = y_file->name;
	return ft_strcmp(y_name, x_name);
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

void quicksort_helper(t_list *head, t_list *tail, int (*f)(void *, void *)) {
	if (head == NULL || head == tail) {
        return;
	}
	t_list *pivot = ft_lstsort(f, head, tail);
	quicksort_helper(head, pivot, f);
	quicksort_helper(pivot->next, tail, f);
	
}

void sort_a(void *lst) {
	quicksort_helper(lst, ft_lstlast(lst), &compare_name);
}

void sort_r(void *lst) {
	quicksort_helper(lst, ft_lstlast(lst), &compare_name_reverse);
}

void sort_t(void *lst) {
	quicksort_helper(lst, ft_lstlast(lst), &compare_time);
}

void sort_single_files(void *lst) {
    quicksort_helper(lst, ft_lstlast(lst), &compare_alphabetical);
}

// void sorting() {
// 	char *arr[] = {"abcde", "werty", "poiuyt", "bcdef", "bcdefg"};
// 	t_list *lst;
// 	lst = NULL;
// 	for (int i = 0; i < 5; i++) {
// 		ft_lstadd_back(&lst, ft_lstnew(new_file(arr[i], "", "")));
// 	}
// 	quicksort_helper(lst, ft_lstlast(lst), &compare_name);
// 	ft_lstiter(lst, &print_files);
// }