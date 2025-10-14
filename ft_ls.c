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

t_padding padding;

int is_dir(char path[]) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

t_file *fetch_file_info(struct dirent *dp, char *path) {
    t_file *file;
    struct stat attr;

    if (stat(path, &attr) == -1)
        return NULL;

    padding.links = __max(padding.links, ft_strlen(ft_itoa(attr.st_nlink)));
    padding.user = __max(padding.user, ft_strlen(ft_itoa(attr.st_uid))); //TODO: Change to getpwuid
    padding.group = __max(padding.group, ft_strlen(ft_itoa(attr.st_gid))); // TODO: Change to get getgrgid
    padding.size = __max(padding.size, ft_strlen(ft_itoa(attr.st_size)));
    file = new_file(ft_strdup(dp->d_name), ft_strdup(path), ft_strdup(ctime(&attr.st_mtime)));
    return file;
}

int get_blocks(char *path) {
    struct stat st;

    stat(path, &st);
    return st.st_size / 2; //Change to st_blocks
}

int print_dirs(char path[]) {
    struct dirent   *dp;
	// t_list          *dirs;
    t_file          *temp_file;
    t_list          *files;
    DIR             *dir;
    char            *file_path;
    
    dir = opendir(path);
    files = NULL;
    if (!dir) {
        struct stat path_stat;
        if (stat(path, &path_stat) == -1) {
            ft_printf("Could not open directory.\n");
            return (1);
        }
        else {
            char *file_name = ft_strchr(path, '/');
            if (file_name == NULL) {
                file_name = ft_strdup(path);
            } else {
                file_name = ft_strdup(++file_name);
            }
            temp_file = new_file(file_name, path, ft_strdup(ctime(&path_stat.st_mtime)));
            ft_lstadd_back(&files, ft_lstnew(temp_file));
        }
    }
    // int total = 0; TODO: add total on linux
    while ((dp = readdir(dir)) != NULL) {
        if (ft_strncmp(dp->d_name, ".", 1) == 0)
            continue;
        
        file_path = ft_strjoin(path, dp->d_name);
        temp_file = fetch_file_info(dp, file_path);
        free(file_path);
        if (!temp_file)
            continue;
        ft_lstadd_back(&files, ft_lstnew(temp_file));
    }
    closedir(dir);
    ft_lstiter(files, &print_files);
    // ft_printf("/////////////");
    // ft_lstiter(dirs, &print_files);

    // free_list(&files);
    return 0;
}

void print_normal_format(void *file) {
    t_file *file_obj = (t_file *) file;
    struct stat file_stat;

    if (stat(file_obj->path, &file_stat) == -1)
        return;
    
    ft_printf("%s ", file_obj->name);
}

void print_permissions(mode_t mode) {
    char permissions[11] = {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '\0'};

    if (S_ISDIR(mode))
        permissions[0] = 'd';
    // else if (S_ISLNK(mode))
    //     permissions[0] = 'l';
    else if (S_ISCHR(mode))
        permissions[0] = 'c';
    else if (S_ISBLK(mode))
        permissions[0] = 'b';
    // else if (S_ISSOCK(mode))
    //     permissions[0] = 's';
    else if (S_ISFIFO(mode))
        permissions[0] = 'p';

    if (mode & S_IRUSR) 
        permissions[1] = 'r';
    if (mode & S_IWUSR) 
        permissions[2] = 'w';
    if (mode & S_IXUSR) 
        permissions[3] = 'x';
    if (mode & S_IRGRP) 
        permissions[4] = 'r';
    if (mode & S_IWGRP) 
        permissions[5] = 'w';
    if (mode & S_IXGRP) 
        permissions[6] = 'x';
    if (mode & S_IROTH) 
        permissions[7] = 'r';
    if (mode & S_IWOTH) 
        permissions[8] = 'w';
    if (mode & S_IXOTH) 
        permissions[9] = 'x';

    ft_printf("%s ", permissions);
}

void pad(int pad, int word_length) {
    if (pad <= word_length)
        return;
    for (int i = 0; i < pad - word_length; i++){
        ft_putchar(' ');
    }
}

void print_long_format(void *file) {
    t_file *file_obj = (t_file *) file;
    struct stat file_stat;

    if (stat(file_obj->path, &file_stat) == -1)
        return;
    print_permissions(file_stat.st_mode);
    char *links = ft_itoa(file_stat.st_nlink);
    pad(padding.links, ft_strlen(links));
    ft_printf("%s ", links);
    pad(padding.user, ft_strlen(ft_itoa(file_stat.st_uid)));
    ft_printf("%i ", file_stat.st_uid);
    pad(padding.group, ft_strlen(ft_itoa(file_stat.st_gid)));
    ft_printf("%i ", file_stat.st_gid);
    pad(padding.size, ft_strlen(ft_itoa(file_stat.st_size)));
    ft_printf("%i ", file_stat.st_size);
    ft_printf("%s ", ctime(&file_stat.st_mtime));
    ft_printf("%s\n", file_obj->name);
}

// t_list get_dirs(char *path, t_flags flags) {
//     struct dirent *dp;
//     t_file *temp_file;
// }

int ft_ls(char *path, t_flags flags) {
    struct dirent   *dp;
    t_file          *temp_file;
    t_list          *files;
    DIR             *dir;
    char            *file_path;
    
    dir = opendir(path);
    files = NULL;
    if (!dir) {
        struct stat path_stat;
        if (stat(path, &path_stat) == -1) {
            stat_error(path, &files);
        }
        else {
            char *file_name = ft_strchr(path, '/');
            if (file_name == NULL) {
                file_name = ft_strdup(path);
            } else {
                file_name = ft_strdup(++file_name);
            }
            temp_file = new_file(file_name, path, ft_strdup(ctime(&path_stat.st_mtime)));
            ft_lstadd_back(&files, ft_lstnew(temp_file));
        }
    }
    while ((dp = readdir(dir)) != NULL) {
        if (!flags.all_files && ft_strncmp(dp->d_name, ".", 1) == 0)
            continue;
        
        file_path = ft_strjoin(path, dp->d_name);
        temp_file = fetch_file_info(dp, file_path);
        free(file_path);
        if (!temp_file)
            continue;
        
        ft_lstadd_back(&files, ft_lstnew(temp_file));
    }
    closedir(dir);

    // Sort files
    if (flags.reverse_order)
        sort_r(files);
    else if (flags.mod_time_order)
        sort_t(files);
    else
        sort_a(files);
    
    // Print files
    if (flags.long_format)
        ft_lstiter(files, &print_long_format);
    else
        ft_lstiter(files, &print_normal_format);
    
    // Recursive print
    t_list *temp = files;
    while (files != NULL && flags.display_recursive) {
        t_file *file = (t_file *)temp->content;
        if (is_dir(file->path) && ft_strncmp(".", file->name, ft_strlen(file->name)) && ft_strncmp("..", file->name, ft_strlen(file->name)))
            ft_ls(file->path, flags);
        temp = temp->next;
    }
    free_list(&files);
    return 0;
}

int main(int argc, char *argv[]) {
    // ft_printf(argv[1]);
    t_flags flags = get_flags(argc, argv);
    print_flags(flags);
    for (int i = 1; i < argc; i++) {
        if (argv[i] && argv[i][0] != '-') {
            char *path = ft_strjoin(argv[i], "/");
            ft_ls(path, flags);
            free(path);
        }
    }

    // sorting();
    
    return 0;
}