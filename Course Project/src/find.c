#include "include.h"
#include <fnmatch.h>  

void execute_find(const char* path, const char* name_filter) {
    search_results.result_count = 0;

    const char* effective_filter = (name_filter == NULL || name_filter[0] == '\0') ? "*" : name_filter;

    char cmd[MAX_CMD];
    snprintf(cmd, MAX_CMD, "find %s -name \"%s\"", path, effective_filter);

    if (settings.filter_only_files)
        strcat(cmd, " -type f");
    else if (settings.filter_only_dirs)
        strcat(cmd, " -type d");

    if (settings.sort_results)
        strcat(cmd, " | sort");

    strcat(cmd, " > temp_results.txt");

    if (command_history.history_count < MAX_HISTORY) {
        strncpy(command_history.history[command_history.history_count], cmd, MAX_CMD - 1);
        command_history.history[command_history.history_count][MAX_CMD - 1] = 0;
        command_history.history_count++;
    }
    else {
        for (int i = 1; i < MAX_HISTORY; i++) {
            strcpy(command_history.history[i - 1], command_history.history[i]);
        }
        strncpy(command_history.history[MAX_HISTORY - 1], cmd, MAX_CMD - 1);
        command_history.history[MAX_HISTORY - 1][MAX_CMD - 1] = 0;
    }

    int ret = system(cmd);
    if (ret != 0) {

        return;
    }

    FILE* file = fopen("temp_results.txt", "r");
    if (file) {
        while (fgets(search_results.results[search_results.result_count], MAX_PATH, file) && search_results.result_count < MAX_RESULTS) {

            search_results.results[search_results.result_count][strcspn(search_results.results[search_results.result_count], "\n")] = 0;
            search_results.result_count++;
        }
        fclose(file);
    }

    remove("temp_results.txt");
}
