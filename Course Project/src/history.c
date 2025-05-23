#include "include.h"

void display_history() {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(0, 0, "Search history:");
    attroff(COLOR_PAIR(1) | A_BOLD);

    for (int i = 0; i < command_history.history_count; i++) {
        mvprintw(2 + i, 0, "%s", command_history.history[i]);
    }

    mvprintw(2 + command_history.history_count + 1, 0, "Press any key to return...");
    getch();
}
