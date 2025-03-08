#ifndef DIRWALK_H
#define DIRWALK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
void walk_directory(const char *dir_path, int show_links, int show_dirs, int show_files, int sort);

#endif
