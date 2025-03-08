#include "dirwalk.h"

int main(int argc, char *argv[]) {
    int show_links = 0, show_dirs = 0, show_files = 0, sort = 0;
    int opt;
    setlocale(LC_COLLATE, "");
    while ((opt = getopt(argc, argv, "ldfs")) != -1) {
        switch (opt) {
            case 'l': show_links = 1; break;
            case 'd': show_dirs = 1; break;
            case 'f': show_files = 1; break;
            case 's': sort = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-d] [-f] [-s] <dir>\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Error: Directory not specified\n");
        return EXIT_FAILURE;
    }

    const char *dir = argv[optind];
    if (!show_links && !show_dirs && !show_files) {
        show_links = show_dirs = show_files = 1;
    }

    if(show_dirs == 1) {
        printf("%s\n", dir);
    }

    walk_directory(dir, show_links, show_dirs, show_files, sort);
    return 0;
}
