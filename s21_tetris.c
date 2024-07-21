#include "gui/cli/frontend.h"

void main_game_cycle(void);

int main(void) {
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  srand(time(NULL));
  curs_set(0);
  timeout(5);
  main_game_cycle();
  endwin();
  return 0;
}

/**
 * Main game function. The heart and brain of the program
 */
void main_game_cycle(void) {
  FULL_INFO;
  while (full_info->state != TERMINATE_STATE) {
    GameInfo_t info = updateCurrentState();
    print_current_game_state(info);
    userInput(user_push(getch()), 0);
  }
  if (full_info->state == TERMINATE_STATE) {
    clear();
    print_terminate(*full_info);
    refresh();
    sleep(1);
  }
  free_field_memory();
}