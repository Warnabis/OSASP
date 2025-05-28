#include "include.h"

void draw_menu(int highlight) {
    const char *options[] = {
        "Change path",
        "Change name filter",
        "Toggle only files [-type f]",
        "Toggle only dirs [-type d]",
        "Toggle only links [-type l]",
        "Toggle sort",
        "Execute Search"
    };

    for (int i = 0; i < 7; i++) {  
        if (highlight == i) {
            attron(COLOR_PAIR(4)); 
        } else {
            attron(COLOR_PAIR(2)); 
        }

        mvprintw(6 + i, 0, "%s %s", options[i],
                 (i == 2 && settings.filter_only_files) ? "[X]" :
                 (i == 3 && settings.filter_only_dirs) ? "[X]" :
                 (i == 4 && settings.filter_only_links) ? "[X]" :
                 (i == 5 && settings.sort_results) ? "[X]" : "");

        attroff(COLOR_PAIR(4)); 
        attroff(COLOR_PAIR(2)); 
    }
}

void display_results() {
    int highlight = 0;
    int choice;

    while (1) {
        clear();
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(0, 0, "Search results (Enter = operations, Q = back)");
        attroff(COLOR_PAIR(1) | A_BOLD);

        for (int i = 0; i < search_results.result_count; i++) {
            if (i == highlight) {
                attron(COLOR_PAIR(4)); 
                mvprintw(2 + i, 0, "%s", search_results.results[i]);
                attroff(COLOR_PAIR(4));
            } else {
                attron(COLOR_PAIR(5)); 
                mvprintw(2 + i, 0, "%s", search_results.results[i]);
                attroff(COLOR_PAIR(5));
            }
        }

        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight - 1 + search_results.result_count) % search_results.result_count;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % search_results.result_count;
                break;
            case 10:
                result_operations(search_results.results[highlight]);
                
                execute_find(settings.path, settings.name_filter);
                break;
            case 'q':
                return;
        }
    }
}

void ui_main_menu_loop() {
    int highlight = 0;
    int choice;
    int menu_size = 7;  

    while (1) {
        clear();
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(0, 0, "NCURSES Shell for find (Q - quit, H - history)");
        attroff(COLOR_PAIR(1) | A_BOLD);

        attron(COLOR_PAIR(5));
        mvprintw(2, 0, "Current path: ");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(3));
        printw("%s", settings.path);

        attron(COLOR_PAIR(5));
        mvprintw(3, 0, "Name filter: ");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(3));
        printw("%s", settings.name_filter);

        attron(COLOR_PAIR(5));
        mvprintw(4, 0, "Only files: ");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(settings.filter_only_files ? 3 : 7)); 
        printw("%s", settings.filter_only_files ? "YES" : "NO");

        attron(COLOR_PAIR(5));
        printw("  |  Only dirs: ");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(settings.filter_only_dirs ? 3 : 7)); 
        printw("%s", settings.filter_only_dirs ? "YES" : "NO");

        attron(COLOR_PAIR(5));
        printw("  |  Only links: ");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(settings.filter_only_links ? 3 : 7)); 
        printw("%s", settings.filter_only_links ? "YES" : "NO");

        attron(COLOR_PAIR(5));
        printw("  |  Sorted: ");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(settings.sort_results ? 3 : 7));  
        printw("%s", settings.sort_results ? "YES" : "NO");
        attroff(COLOR_PAIR(3));

        draw_menu(highlight);

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
                    case 0:
                        echo();
                        mvprintw(14, 0, "Enter new path: ");
                        getnstr(settings.path, MAX_PATH - 1);
                        noecho();
                        break;
                    case 1:
                        echo();
                        mvprintw(14, 0, "Enter name filter: ");
                        getnstr(settings.name_filter, MAX_PATH - 1);
                        noecho();
                        break;
                    case 2:
                        settings.filter_only_files = !settings.filter_only_files;
                        if (settings.filter_only_files) 
                        settings.filter_only_dirs = 0;
                        settings.filter_only_links = 0;
                        break;
                    case 3:
                        settings.filter_only_dirs = !settings.filter_only_dirs;
                        if (settings.filter_only_dirs) 
                        settings.filter_only_files = 0;
                        settings.filter_only_links = 0;
                        break;
                    case 4:
                        settings.filter_only_links = !settings.filter_only_links;
                        if (settings.filter_only_links) 
                        settings.filter_only_files = 0;
                        settings.filter_only_dirs = 0;
                        break;
                    case 5:
                        settings.sort_results = !settings.sort_results;
                        break;
                    case 6:
                        execute_find(settings.path, settings.name_filter);
                        display_results();
                        break;
                }
                break;
            case 'q':
                return;
            case 'h':
                display_history();
                break;
        }
    }
}
