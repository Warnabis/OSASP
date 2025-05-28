#include "include.h"

SearchSettings settings = {
    .path = "/",
    .name_filter = "*",
    .filter_only_files = 0,
    .filter_only_dirs = 0,
    .filter_only_links = 0,
    .sort_results = 0
};

SearchResults search_results = {
    .result_count = 0
};

CommandHistory command_history = {
    .history_count = 0
};

int main() {
    setlocale(LC_ALL, "");

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();

    init_pair(1, COLOR_YELLOW, COLOR_BLACK);   
    init_pair(2, COLOR_CYAN, COLOR_BLACK);     
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   
    init_pair(4, COLOR_WHITE, COLOR_BLUE);     
    init_pair(5, COLOR_WHITE, COLOR_BLACK);   
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);   
    init_pair(7, COLOR_RED, COLOR_BLACK);      

    ui_main_menu_loop();

    endwin();
    return 0;
}
