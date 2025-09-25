#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joint;
	size_t	sizes1;
	size_t	sizes2;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	sizes1 = strlen(s1);
	sizes2 = strlen(s2);
	joint = malloc(sizes1 + sizes2 + 1);
	if (!joint)
		return (NULL);
	joint = memcpy(joint, s1, sizes1);
	i = 0;
	while (i < sizes2)
	{
		joint[i + sizes1] = s2[i];
		i++;
	}
	joint[sizes1 + sizes2] = '\0';
	return (joint);
}

int is_dir(char path[]) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

int print_dirs(char path[]) {
    DIR *dir = opendir(path);
    if (!dir) {
        printf("Could not open directory.\n");
        return (1);
    }
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;
        printf("%s", dp->d_name);
        if (is_dir(ft_strjoin(path, ft_strjoin("/", dp->d_name))))
            printf("/");
        printf("\t");
    }
    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
    print_dirs(".");
    
    return 0;
}