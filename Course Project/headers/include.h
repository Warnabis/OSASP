#ifndef INCLUDE_H
#define INCLUDE_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <locale.h>

#define MAX_PATH 256
#define MAX_CMD 512
#define MAX_RESULTS 100
#define MAX_HISTORY 10

typedef struct {
    char path[MAX_PATH];
    char name_filter[MAX_PATH];
    int filter_only_files;
    int filter_only_dirs;
    int sort_results;
} SearchSettings;

typedef struct {
    char results[MAX_RESULTS][MAX_PATH];
    int result_count;
} SearchResults;

typedef struct {
    char history[MAX_HISTORY][MAX_CMD];
    int history_count;
} CommandHistory;

extern SearchSettings settings;
extern SearchResults search_results;
extern CommandHistory command_history;

void draw_menu(int highlight);
void display_results();
void display_history();

void execute_find(const char *path, const char *name_filter);

void result_operations(char *filename);

#endif
