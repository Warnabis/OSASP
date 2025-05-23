#include "include.h"

void result_operations(char *filename) {
    int highlight = 0;
    int choice;
    char cmd[MAX_CMD];
    char msg[128];

    clear();
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(0, 0, "File operations: %s", filename);
    attroff(COLOR_PAIR(2) | A_BOLD);

    const char *operations[] = {"Open", "Delete", "Copy", "Back"};
    int menu_size = 4;

    while (1) {
        clear();  
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(0, 0, "File operations: %s", filename);
        attroff(COLOR_PAIR(1) | A_BOLD);

        for (int i = 0; i < menu_size; i++) {
            if (i == highlight) {
                attron(COLOR_PAIR(4)); 
            } else {
                attron(COLOR_PAIR(5)); 
            }

            mvprintw(2 + i, 0, "%d. %s", i + 1, operations[i]);
            attroff(COLOR_PAIR(4)); 
            attroff(COLOR_PAIR(5)); 
        }

        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight - 1 + menu_size) % menu_size;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % menu_size;
                break;
            case 10: 
                switch (highlight) {
                    case 0: {  
                        snprintf(cmd, MAX_CMD, "xdg-open \"%s\"", filename);
                        system(cmd);
                        strcpy(msg, "File opened successfully.");
                        break;
                    }
                    case 1: {  
                        struct stat st;
                        if (stat(filename, &st) != 0) {
                            strcpy(msg, "Failed to access file.");
                            mvprintw(menu_size + 6, 0, "%s Press any key to continue...", msg);
                            getch();
                            break;
                        }
                    
                        if (S_ISDIR(st.st_mode)) {
                            snprintf(cmd, MAX_CMD, "rm -r \"%s\"", filename);  
                        } else {
                            snprintf(cmd, MAX_CMD, "rm \"%s\"", filename);    
                        }
                    
                        if (system(cmd) == 0) {
                            strcpy(msg, "File deleted successfully.");
                            for (int i = 0; i < search_results.result_count; i++) {
                                if (strcmp(search_results.results[i], filename) == 0) {
                                    for (int j = i; j < search_results.result_count - 1; j++) {
                                        strcpy(search_results.results[j], search_results.results[j + 1]);
                                    }
                                    search_results.result_count--;
                                    break;
                                }
                            }
                        } else {
                            strcpy(msg, "Failed to delete file.");
                        }
                        break;
                    }
                    case 2: { 
                        echo();
                        char dest[MAX_PATH];
                        mvprintw(menu_size + 4, 0, "Enter destination path: ");
                        getnstr(dest, MAX_PATH - 1);
                        noecho();

                        snprintf(cmd, MAX_CMD, "cp -r \"%s\" \"%s\"", filename, dest);
                        if (system(cmd) == 0) {
                            strcpy(msg, "File copied successfully.");
                        } else {
                            strcpy(msg, "Failed to copy file.");
                        }
                        break;
                    }
                    case 3: 
                        return;
                }
                mvprintw(menu_size + 6, 0, "%s Press any key to continue...", msg);
                getch();
                break;
        }
    }
}
