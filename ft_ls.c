/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtravez <mtravez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:51:15 by mtravez           #+#    #+#             */
/*   Updated: 2025/10/15 21:02:06 by mtravez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_padding padding;

int is_dir(char path[]) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

t_file *fetch_file_info(char *name, char *path, long *total) {
    struct stat attr;

    if (lstat(path, &attr) == -1)
        return NULL;

    char *link_number = ft_itoa(attr.st_nlink);
    char *size_number = ft_itoa(attr.st_size);
    padding.links = MAX(padding.links, (int)ft_strlen(link_number));
    padding.user = MAX(padding.user, (int)ft_strlen(getpwuid(attr.st_uid)->pw_name));
    padding.group = MAX(padding.group, (int)ft_strlen(getgrgid(attr.st_gid)->gr_name));
    padding.size = MAX(padding.size, (int)ft_strlen(size_number));
    free(link_number);
    free(size_number);
    
    *total += attr.st_blocks;
    return new_file(ft_strdup(name), ft_strdup(path), attr.st_mtime);
}

int get_blocks(char *path) {
    struct stat st;

    stat(path, &st);
    return st.st_size / 2; //Change to st_blocks
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
    else if (S_ISLNK(mode))
        permissions[0] = 'l';
    else if (S_ISCHR(mode))
        permissions[0] = 'c';
    else if (S_ISBLK(mode))
        permissions[0] = 'b';
    else if (S_ISSOCK(mode))
        permissions[0] = 's';
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

char *get_mtime(time_t mtime) {
    char *time = ctime(&mtime);
    if (!time)
        return "";
    time[ft_strlen(time) - 9] = 0;
    return &time[4];
}

void print_long_format(void *file) {
    t_file *file_obj = (t_file *) file;
    struct stat file_stat;

    if (lstat(file_obj->path, &file_stat) == -1)
        return;
    print_permissions(file_stat.st_mode);
    char *links = ft_itoa(file_stat.st_nlink);
    char *size = ft_itoa(file_stat.st_size);
    pad(padding.links, ft_strlen(links));
    ft_printf("%s ", links);
    pad(padding.user, ft_strlen(getpwuid(file_stat.st_uid)->pw_name));
    ft_printf("%s ", getpwuid(file_stat.st_uid)->pw_name);
    pad(padding.group, ft_strlen(getgrgid(file_stat.st_gid)->gr_name));
    ft_printf("%s ", getgrgid(file_stat.st_gid)->gr_name);
    pad(padding.size, ft_strlen(size));
    ft_printf("%i ", file_stat.st_size);
    free(links);
    free(size);
    ft_printf("%s ", get_mtime(file_obj->mtime));
    ft_printf("%s", file_obj->name);
    if (S_ISLNK(file_stat.st_mode)) {
        char buf[PATH_MAX];
        ssize_t len = readlink(file_obj->path, buf, sizeof(buf) - 1);
        if (len == -1) {
            ft_putchar('\n');
            return;
        }
        buf[len] = '\0';
        ft_printf(" -> %s", buf);
    }
    ft_putchar('\n');
}

int ft_ls(char *path, t_flags flags, int multiple) {
    struct dirent   *dp;
    t_file          *temp_file;
    t_list          *files;
    DIR             *dir;
    char            *file_path;
    char            *dir_path;
    t_padding       save_padding;
    int             error_code = 0;
    
    save_padding = padding;
    dir_path = ft_strjoin(path, "/");
    dir = opendir(dir_path);
    files = NULL;
    if (!dir) {
        stat_error(dir_path, &files);
        free(dir_path);
        errno = 0;
        return(1);
    }
    long total = 0;
    while ((dp = readdir(dir)) != NULL) {
        if (!flags.all_files && ft_strncmp(dp->d_name, ".", 1) == 0)
            continue;
        
        file_path = ft_strjoin(dir_path, dp->d_name);
        temp_file = fetch_file_info(dp->d_name, file_path, &total);
        free(file_path);
        if (!temp_file)
            continue;
        
        ft_lstadd_back(&files, ft_lstnew(temp_file));
    }
    if (errno != 0) {
        ft_putstr_fd("ft_ls: reading directory '", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd("': ", STDERR_FILENO);
        perror(NULL);
        closedir(dir);
        free(dir_path);
        free_list(&files);
        errno = 0;
        return 2;
    }
    closedir(dir);
    
    // Sort files
    if (flags.reverse_order)
    sort_r(files);
    else if (flags.mod_time_order)
    sort_t(files);
    else {
        sort_a(files);
    }
    
    // Print files
    if (flags.long_format) {
        ft_printf("total %i\n", (total / 2));
        ft_lstiter(files, &print_long_format);
    }
    else {
        ft_lstiter(files, &print_normal_format);
        ft_printf("\n");
    }
    
    padding = save_padding;
    
    // Recursive print
    t_list *temp = files;
    while (temp != NULL && flags.display_recursive) {
        t_file *file = (t_file *)temp->content;
        if (is_dir(file->path) && ft_strncmp(".", file->name, ft_strlen(file->name)) && ft_strncmp("..", file->name, ft_strlen(file->name))) {
            if (multiple)
                ft_printf("\n%s:\n", file->path);
            error_code = ft_ls(file->path, flags, multiple);
        }
        temp = temp->next;
    }
    free_list(&files);
    free(dir_path);
    return error_code;
}

void clear_node(void *node) {
    if (node)
        return;
    return;
}

int main(int argc, char *argv[]) {
    t_flags flags = get_flags(argc, argv);
    t_list *dirs = NULL;
    t_list *files = NULL;
    t_list *errors = NULL;
    int error_code = 0;
    long total = 0;
    int first_printing = 1;
    
    for (int i = 1; i < argc; i++) {
        if (!argv[i] || argv[i][0] == '-') {
            continue;
        }
        struct stat content;
        if (stat(argv[i], &content) == -1) {
            ft_lstadd_back(&errors, ft_lstnew(argv[i]));
            error_code = 1;
        }
        else if (S_ISDIR(content.st_mode)) {
            ft_lstadd_back(&dirs, ft_lstnew(fetch_file_info(argv[i], argv[i], &total)));
        }
        else
            ft_lstadd_back(&files, ft_lstnew(fetch_file_info(argv[i], argv[i], &total)));
    }

    for (t_list *e = errors; e; e = e->next) {
        ft_putstr_fd("ft_ls: cannot access '", STDERR_FILENO);
        ft_putstr_fd((char *)e->content, STDERR_FILENO);
        ft_putstr_fd("': ", STDERR_FILENO);
        perror(NULL);
        errno = 0;
    }
    
    if (ft_lstsize(files) >= 1) {
        sort_a(files);
    
        if (flags.reverse_order)
            sort_r(files);
        if (flags.mod_time_order)
            sort_t(files);
    
        if (flags.long_format) {
            ft_lstiter(files, &print_long_format);
        }
        else
            ft_lstiter(files, &print_normal_format);
        ft_putchar('\n');
    }

    if (ft_lstsize(dirs) >= 1) {
        sort_a(dirs);
    
        if (flags.reverse_order)
            sort_r(dirs);
        if (flags.mod_time_order)
            sort_t(dirs);
    }
    int multiple = (ft_lstsize(dirs) + ft_lstsize(files)) > 1;
    if (flags.display_recursive || ft_lstsize(errors) > 0)
        multiple = 1;
    for (t_list *d = dirs; d; d = d->next) {
        if (!first_printing)
            ft_printf("\n");
        else {
            first_printing = 0;
        }
        t_file *dir = (t_file *)d->content;
        if (multiple)
            ft_printf("%s:\n", dir->path);
        int code = ft_ls(dir->path, flags, multiple);
        error_code = MAX(code, error_code);
    }
    if (ft_lstsize(dirs) == 0 && ft_lstsize(files) == 0 && ft_lstsize(errors) == 0) {        
        if (multiple) {
            ft_printf(".:\n");
        }
        int code = ft_ls(".", flags, multiple);
        error_code = MAX(code, error_code);
    }
    free_list(&files);
    free_list(&dirs);
    ft_lstclear(&errors, &clear_node);
    
    return error_code;
}