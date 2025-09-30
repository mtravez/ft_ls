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

t_file *fetch_file_info(struct dirent *dp, char *path) {
    t_file *file;
    struct stat attr;

    ft_printf("%s\n", path);
    if (stat(path, &attr) == -1)
        return NULL;
    file = new_file(ft_strdup(dp->d_name), ft_strdup(path), ft_strdup(ctime(&attr.st_mtime)));
    return file;
}

int print_dirs(char path[]) {
    struct dirent   *dp;
	// t_list          *dirs;
    t_file          *new_file;
    t_list          *files;
    DIR             *dir;
    char            *file_path;
    
    dir = opendir(path);
    files = NULL;
    if (!dir) {
        ft_printf("Could not open directory.\n");
        return (1);
    }
    while ((dp = readdir(dir)) != NULL) {
        if (ft_strncmp(dp->d_name, ".", 1) == 0)
        continue;
        
        file_path = ft_strjoin(path, dp->d_name);
        new_file = fetch_file_info(dp, file_path);
        free(file_path);
        if (!new_file)
            continue;
        ft_lstadd_back(&files, ft_lstnew(new_file));
    }
    closedir(dir);
    ft_lstiter(files, &print_files);
    // ft_printf("/////////////");
    // ft_lstiter(dirs, &print_files);
    return 0;
}

int main(int argc, char *argv[]) {
	
	if (argc < 2) {
		return 1;
	}
    // ft_printf(argv[1]);
    char *path = ft_strjoin(argv[1], "/");
    print_dirs(path);
    free(path);

    // sorting();
    
    return 0;
}