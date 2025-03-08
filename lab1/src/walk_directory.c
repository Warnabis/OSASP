#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <locale.h>

int compare_strings(const struct dirent **a, const struct dirent **b) {
    return strcoll((*a)->d_name, (*b)->d_name);
}

void walk_directory(const char *dir_path, int show_links, int show_dirs, int show_files, int sort) {
    struct dirent **namelist;
    int n;

    n = scandir(dir_path, &namelist, NULL, sort ? compare_strings : NULL);
    if (n < 0) {
        perror("scandir");
        return;
    }

    for (int i = 0; i < n; i++) {
        struct dirent *entry = namelist[i];

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            free(namelist[i]);
            continue;
        }

        char full_path[2048];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat type;
        if (lstat(full_path, &type) == -1) {
            perror("lstat");
            free(namelist[i]);
            continue;
        }

        int is_link = S_ISLNK(type.st_mode);
        int is_dir = S_ISDIR(type.st_mode);
        int is_file = S_ISREG(type.st_mode);

        if ((show_links && is_link) || (show_dirs && is_dir) || (show_files && is_file)) {
            printf("%s\n", full_path);
        }

        if (is_dir) {
            walk_directory(full_path, show_links, show_dirs, show_files, sort);
        }

        free(namelist[i]);
    }

    free(namelist);
}
