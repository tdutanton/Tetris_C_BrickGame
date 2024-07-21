#include "frontend.h"

/**
 * Names of fields (boxes)
 */
WIN frame, main_field, high_score_field, score_field, level_field, speed_field,
    next_fig_field;

/**
 * Text on fields
 */
char* LOGOS_NAMES[] = {"HIGH SCORE", "SCORE",       "LEVEL",
                       "SPEED",      "NEXT FIGURE", NULL};

/**
 * Draw fields on a console screen
 */
void paint_boxes(void) {
  frame = newwin(ROWS + 2, COLS_FRONT + 2, 2, 2);
  int main_begin_x = 0;
  int main_begin_y = 0;
  int main_end_x = 0;
  getbegyx(frame, main_begin_y, main_begin_x);
  main_end_x = getmaxx(frame);
  main_field = newwin(ROWS, COLS_FRONT, main_begin_y + 1, main_begin_x + 1);
  high_score_field = newwin(4, 20, frame->_begy, main_end_x + 2);
  score_field =
      newwin(4, 20, high_score_field->_begy + 4, high_score_field->_begx);
  level_field = newwin(4, 20, score_field->_begy + 4, score_field->_begx);
  speed_field = newwin(4, 20, level_field->_begy + 4, level_field->_begx);
  next_fig_field = newwin(6, 20, speed_field->_begy + 4, speed_field->_begx);
  WIN wins_arr[] = {frame,       high_score_field, score_field, level_field,
                    speed_field, next_fig_field,   NULL};
  int i = 0;
  for (WIN* w_arr = wins_arr; *w_arr != NULL; w_arr++) {
    box(*w_arr, 0, 0);
    mvwprintw(wins_arr[i + 1], 1, 1, "%s", LOGOS_NAMES[i]);
    i++;
  }
  mvprintw(getmaxy(frame) + 2, main_begin_x,
           "ENTER - start, P - pause, Q - end game\n  LEFT - step left, RIGHT "
           "- step right, DOWN - fall\n  SPACE - action (turn figure)");
}

/**
 * Print main information on a screen
 */
void print_stats(GameInfo_t info) {
  mvwprintw(high_score_field, 2, 1, "%d", info.high_score);
  mvwprintw(score_field, 2, 1, "%d", info.score);
  mvwprintw(level_field, 2, 1, "%d", info.level);
  mvwprintw(speed_field, 2, 1, "%d", info.level);
  fill_next_figure(info);
  fill_main_field(info);
  refresh_windows();
}

/**
 * Print greetings when game started
 */
void print_greeting(void) { mvprintw(5, 5, "-= PRESS ENTER TO START =-"); }

/**
 * Print game over text when game is over
 */
void print_game_over(full_game_info_t full_info) {
  mvprintw(5, 5, "     -= GAME OVER =-");
  mvprintw(7, 5, "    YOUR SCORE: %d", full_info.info.score);
  mvprintw(9, 5, "PRESS ENTER TO START AGAIN");
}

/**
 * Print 'Pause' text on a screen
 */
void print_pause(void) { mvprintw(5, 5, "-= PAUSE =-"); }

/**
 * Print text on a screen when user chose a 'Quit' action
 * \param full_info a value of structure with full game parameters
 */
void print_terminate(full_game_info_t full_info) {
  mvprintw(5, 5, "-= GOOD BYE =-");
  mvprintw(6, 5, "YOUR SCORE: %d", full_info.info.score);
}

/**
 * Fill main field with blocks
 * \param info a value of structure with game parameters
 */
void fill_main_field(GameInfo_t info) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      mvwaddch(main_field, i, j * 2,
               (info.field[i][j] == CURRENT_FIGURE_BLOCK ||
                info.field[i][j] == ATTACHED_BLOCK)
                   ? '['
                   : ' ');
      mvwaddch(main_field, i, j * 2 + 1,
               (info.field[i][j] == CURRENT_FIGURE_BLOCK ||
                info.field[i][j] == ATTACHED_BLOCK)
                   ? ']'
                   : ' ');
    }
  }
  wnoutrefresh(main_field);
  doupdate();
}

/**
 * Fill next-figure field with blocks
 * \param info a value of structure with game parameters
 */
void fill_next_figure(GameInfo_t info) {
  FULL_INFO;
  for (int field_i = 2,
           fig_i = random_fig_to_spawn(full_info->next_fig_id).y * -1;
       fig_i < MAX_FIG_ROWS; field_i++, fig_i++) {
    for (int field_j = 3, fig_j = 0; fig_j < MAX_FIG_COLS; field_j++, fig_j++) {
      mvwaddch(next_fig_field, field_i, field_j * 2,
               (info.next[fig_i][fig_j] == CURRENT_FIGURE_BLOCK) ? '[' : ' ');
      mvwaddch(next_fig_field, field_i, field_j * 2 + 1,
               (info.next[fig_i][fig_j] == CURRENT_FIGURE_BLOCK) ? ']' : ' ');
    }
  }
  wnoutrefresh(next_fig_field);
  doupdate();
}

/**
 * Refreshing all fields (boxes) on a screen
 */
void refresh_windows(void) {
  WIN wins_arr[] = {main_field,  frame,       high_score_field,
                    score_field, level_field, speed_field,
                    NULL};
  for (WIN* w_arr = wins_arr; *w_arr != NULL; w_arr++) {
    wnoutrefresh(*w_arr);
    doupdate();
  }
}

/**
 * Main function which 'print' current info depending on a current game state
 * \param info a value of structure with game parameters
 */
void print_current_game_state(GameInfo_t info) {
  FULL_INFO;
  switch (full_info->state) {
    case START_STATE:
      erase();
      print_greeting();
      wnoutrefresh(stdscr);
      doupdate();
      break;
    case SPAWN_STATE:
      paint_boxes();
      break;
    case GAME_OVER_STATE:
      erase();
      print_game_over(*full_info);
      wnoutrefresh(stdscr);
      doupdate();
      break;
    case PAUSE_STATE:
      print_pause();
      paint_boxes();
      break;
    default:
      print_stats(info);
      break;
  }
}