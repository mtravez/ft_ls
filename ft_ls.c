/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtravez <mtravez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:51:15 by mtravez           #+#    #+#             */
/*   Updated: 2025/09/29 15:02:00 by mtravez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int is_dir(char path[]) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

int print_dirs(char path[]) {
    DIR *dir = opendir(path);
    if (!dir) {
        ft_printf("Could not open directory.\n");
        return (1);
    }
    struct dirent *dp;
	// t_list *names;
	t_list *dirs;
	char **files;
	files = malloc(dir->__dd_len * sizeof(char *));
	int i = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (ft_strncmp(dp->d_name, ".", ft_strlen(dp->d_name)) == 0 || ft_strncmp(dp->d_name, "..", ft_strlen(dp->d_name)) == 0)
            continue;
		files[i] = malloc(ft_strlen(dp->d_name) + 1);
		files[i] = dp->d_name;
        if (is_dir(ft_strjoin(path, ft_strjoin("/", dp->d_name))))
			dirs = ft_lstnew(dp->d_name);
    }
	ft_printf("%s", dp->d_name);
    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
	
	if (argc < 2) {
		return 1;
	}
    ft_printf(argv[1]);
    
    sorting();
    
    return 0;
}