#include "backend.h"
/**
 * Figure O - Square
 */
FIGURE fig_O = {0,
                'O',
                2,
                2,
                START_X_POS_SPAWN,
                -1,
                horizontal,
                {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}};

/**
 * Figure I - line
 */
FIGURE fig_I = {1,
                'I',
                1,
                4,
                START_X_POS_SPAWN,
                -1,
                horizontal,
                {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

/**
 * Figure J
 */
FIGURE fig_J = {2,
                'J',
                2,
                3,
                START_X_POS_SPAWN,
                -1,
                horizontal,
                {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}};

/**
 * Figure L
 */
FIGURE fig_L = {3,
                'L',
                2,
                3,
                START_X_POS_SPAWN,
                -1,
                horizontal,
                {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}};

/**
 * Figure S
 */
FIGURE fig_S = {4,
                'S',
                2,
                3,
                START_X_POS_SPAWN,
                -2,
                horizontal,
                {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}}};

/**
 * Figure T
 */
FIGURE fig_T = {5,
                'T',
                2,
                3,
                START_X_POS_SPAWN,
                -1,
                horizontal,
                {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}};

/**
 * Figure Z
 */
FIGURE fig_Z = {6,
                'Z',
                2,
                3,
                START_X_POS_SPAWN,
                -2,
                horizontal,
                {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}}};

/**
 * Array with all figures in game
 */
FIGURE* figs[] = {&fig_O, &fig_I, &fig_J, &fig_L, &fig_S, &fig_T, &fig_Z, NULL};

/**
 * One of the main functions. Returns a pointer to the current state of the
 * structure
 */
full_game_info_t* update_ptr_full(void) {
  static full_game_info_t full_info = {0};
  if (full_info.info.field == NULL)
    full_info.info.field = init_empty_area(ROWS, COLS);
  if (full_info.info.next == NULL)
    full_info.info.next = init_empty_area(MAX_FIG_ROWS, MAX_FIG_COLS);
  return &full_info;
}

/**
 * Main geme function. Returns a current state of the structure from pointer of
 * the structure with all parameters
 */
GameInfo_t updateCurrentState(void) {
  GameInfo_t info = {0};
  FULL_INFO;
  info.field = init_empty_area(ROWS, COLS);
  copy_area(full_info->info.field, info.field, ROWS, COLS);
  info.next = init_empty_area(MAX_FIG_ROWS, MAX_FIG_COLS);
  copy_area(full_info->info.next, info.next, MAX_FIG_ROWS, MAX_FIG_COLS);
  info.high_score = full_info->info.high_score;
  info.score = full_info->info.score;
  info.score = full_info->info.score;
  info.level = full_info->info.level;
  info.speed = full_info->info.speed;
  return info;
}

/**
 * Make empty areas (fields) for main game field and field for nex figure
 * \param rows count of field rows
 * \param cols count of field columns
 */
int** init_empty_area(int rows, int cols) {
  int** empty_field = (int**)malloc(rows * sizeof(int*));
  if (empty_field) {
    for (int i = 0; i < rows;) {
      empty_field[i] = (int*)malloc(cols * sizeof(int));
      if (empty_field[i]) i++;
    }
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      empty_field[i][j] = EMPTY_BLOCK;
    }
  }
  return empty_field;
}

/**
 * Make a copy of current field to new field
 * \param current pointer to first elemrnt of current field
 * \param new_area pointer to first element of new field
 * \param rows count of field rows
 * \param cols count of field columns
 */
void copy_area(int** current, int** new_area, int rows, int cols) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      new_area[i][j] = current[i][j];
    }
}

/**
 * Make a copy of current area with figure to new area
 * \param current_figure pointer to first elemrnt of current field
 * \param new_area pointer to first element of new field
 */
void copy_figure(int current_figure[MAX_FIG_ROWS][MAX_FIG_COLS],
                 int** new_area) {
  for (int i = 0; i < MAX_FIG_ROWS; i++)
    for (int j = 0; j < MAX_FIG_COLS; j++) {
      new_area[i][j] = current_figure[i][j];
    }
}

/**
 * Кeystroke recognition
 * \param key code of key that user push on keyboard
 */
UserAction_t user_push(int key) {
  UserAction_t push = -1;
  switch (key) {
    case KEY_ENTER:
    case '\n':
      push = Start;
      break;
    case 'P':
    case 'p':
      push = Pause;
      break;
    case 'Q':
    case 'q':
      push = Terminate;
      break;
    case KEY_LEFT:
      push = Left;
      break;
    case KEY_RIGHT:
      push = Right;
      break;
    case KEY_DOWN:
      push = Down;
      break;
    case KEY_F0:
    case SPACEBAR:
      push = Action;
  }
  return push;
}

/**
 * Recognition of pressed keys, changing current states depending on the pressed
 * key \param action code of key that user push on keyboard \param hold
 * represent of holding a button
 */
void userInput(UserAction_t action, bool hold) {
  (void)hold;
  FULL_INFO;
  switch (full_info->state) {
    case START_STATE:
      start_actions(full_info, action);
      break;
    case SPAWN_STATE:
      spawn_figure();
      full_info->state = MOVING_STATE;
      break;
    case MOVING_STATE:
      moving_actions(full_info, action);
      break;
    case SHIFTING_STATE:
      step_down();
      if (collision_down_check(*full_info) == collision_down)
        full_info->state = ATTACHING_STATE;
      else
        full_info->state = MOVING_STATE;
      break;
    case ATTACHING_STATE:
      attaching_actions(full_info);
      break;
    case PAUSE_STATE:
      switch (action) {
        case Pause:
          pause_process(full_info);
          break;
        case Terminate:
          update_high_score();
          full_info->state = TERMINATE_STATE;
          break;
        default:
          break;
      }
      break;
    case GAME_OVER_STATE:
      switch (action) {
        case Start:
          full_info->state = START_STATE;
          break;
        default:
          break;
      }
    default:
      break;
  }
}

/**
 * Proccessing of moving actions depending on keyboard keys (user actions)
 * \param full_info pointer of full structure with game parameters
 * \param action code of key that user push on keyboard
 */
void moving_actions(full_game_info_t* full_info, UserAction_t action) {
  switch (action) {
    case Left:
      step_left();
      break;
    case Right:
      step_right();
      break;
    case Down:
      full_info->state = ATTACHING_STATE;
      fall_down();
      break;
    case Action:
      rotate();
      break;
    case Pause:
      full_info->state = PAUSE_STATE;
      full_info->info.pause = 1;
      break;
    case Terminate:
      full_info->state = TERMINATE_STATE;
      update_high_score();
      break;
    default:
  }
  if ((update_time() - full_info->timer) >= full_info->info.speed) {
    full_info->state = SHIFTING_STATE;
    full_info->timer = update_time();
  }
}

/**
 * Proccessing of attaching actions depending on available to spawn new figure
 * \param full_info pointer of full structure with game parameters
 */
void attaching_actions(full_game_info_t* full_info) {
  copy_only_attached_blocks();
  attaching_figure();
  check_and_shift_filled_line();
  set_level();
  set_speed();
  full_info->state =
      (is_not_attached_on_top(*full_info) ? SPAWN_STATE : GAME_OVER_STATE);
}

/**
 * Proccessing of start actions depending on keyboard keys (user actions)
 * \param full_info pointer of full structure with game parameters
 * \param action code of key that user push on keyboard
 */
void start_actions(full_game_info_t* full_info, UserAction_t action) {
  switch (action) {
    case Start:
      prepare_start_field();
      full_info->state = SPAWN_STATE;
      break;
    case Terminate:
      full_info->state = TERMINATE_STATE;
      break;
    default:
      full_info->state = START_STATE;
      break;
  }
}

/**
 * Change pause state
 * \param full_info pointer of full structure with game parameters
 */
void pause_process(full_game_info_t* full_info) {
  if (!full_info->info.pause) {
    full_info->info.pause = 1;
    full_info->state = PAUSE_STATE;
  } else if (full_info->info.pause) {
    full_info->info.pause = 0;
    full_info->state = MOVING_STATE;
  }
}

/**
 * Check if there's any non-empty blocks on top of the main field
 * \param full_info a value of structure with game parameters
 */
int is_not_attached_on_top(full_game_info_t full_info) {
  int available = 1;
  for (int i = 0; i < COLS && available; i++) {
    if (full_info.info.field[0][i] == ATTACHED_BLOCK) available = 0;
  }
  return available;
}

/**
 * Attaching current figure on the main field
 */
void attaching_figure(void) {
  FULL_INFO;
  for (int i = 0; i < MAX_FIG_ROWS; i++) {
    for (int j = 0; j < MAX_FIG_COLS; j++) {
      if (full_info->figure.fig[i][j] == CURRENT_FIGURE_BLOCK) {
        full_info->info
            .field[full_info->figure.y + i][full_info->figure.x + j] =
            ATTACHED_BLOCK;
      }
    }
  }
}

/**
 * Get random Figure id spawn
 */
int get_random_fig_id(void) { return (rand() % 7); }

/**
 * Get Figure from generated random id
 * \param id Figure id
 */
FIGURE random_fig_to_spawn(int id) {
  FIGURE spawned = {0};
  int flag = 0;
  for (FIGURE** figs_arr = figs; figs_arr != NULL && flag == 0; figs_arr++) {
    if ((*figs_arr)->fig_id == id) {
      spawned = **figs_arr;
      flag = 1;
    }
  }
  return spawned;
}

/**
 * Initialization start parameters of game structure
 */
void prepare_start_field(void) {
  FULL_INFO;
  read_high_score();
  full_info->info.level = 1;
  full_info->info.score = 0;
  full_info->info.pause = 0;
  full_info->info.speed = START_SPEED;
  full_info->next_fig_id = RANDOM_ID;
  FIGURE tmp_fig = random_fig_to_spawn(full_info->next_fig_id);
  copy_figure(tmp_fig.fig, full_info->info.next);
  clean_field();
}

/**
 * Free memory from main field and next figure field values
 */
void free_field_memory(void) {
  FULL_INFO;
  if (full_info->info.field) {
    for (int i = 0; i < ROWS; i++) {
      free(full_info->info.field[i]);
    }
    free(full_info->info.field);
  }
  if (full_info->info.next) {
    for (int i = 0; i < MAX_FIG_ROWS; i++) {
      free(full_info->info.next[i]);
    }
    free(full_info->info.next);
  }
}

/**
 * Spawn a new figure on a field
 */
void spawn_figure(void) {
  FULL_INFO;
  full_info->timer = update_time();
  full_info->figure = random_fig_to_spawn(full_info->next_fig_id);
  reset_next_field();
  full_info->next_fig_id = RANDOM_ID;
  FIGURE tmp_fig = random_fig_to_spawn(full_info->next_fig_id);
  copy_figure(tmp_fig.fig, full_info->info.next);
  for (int i = 0, i_figure = full_info->figure.y * -1; i_figure < MAX_FIG_ROWS;
       i++, i_figure++) {
    for (int j = START_X_POS_SPAWN, j_figure = 0; j_figure < MAX_FIG_COLS;
         j++, j_figure++) {
      full_info->info.field[i][j] =
          (full_info->figure.fig[i_figure][j_figure] == CURRENT_FIGURE_BLOCK
               ? full_info->figure.fig[i_figure][j_figure]
               : full_info->info.field[i][j]);
    }
  }
  REFRESH_FIGURE;
}

/**
 * Cleaning a next figure field
 */
void reset_next_field(void) {
  FULL_INFO;
  full_info->next_fig_id = -1;
  for (int i = 0; i < MAX_FIG_ROWS; i++) {
    for (int j = 0; j < MAX_FIG_COLS; j++) {
      full_info->info.next[i][j] = EMPTY_BLOCK;
    }
  }
}

/**
 * Place current Figure on a main field
 */
void place_fig_on_field(void) {
  FULL_INFO;
  for (int i = 0; i < MAX_FIG_ROWS; i++) {
    for (int j = 0; j < MAX_FIG_COLS; j++) {
      if (full_info->figure.fig[i][j] == CURRENT_FIGURE_BLOCK) {
        full_info->info
            .field[full_info->figure.y + i][full_info->figure.x + j] =
            CURRENT_FIGURE_BLOCK;
      }
    }
  }
}

/**
 * Clean main field from current Figure and leaves only attached figures
 */
void copy_only_attached_blocks(void) {
  FULL_INFO;
  int** tmp_field = init_empty_area(ROWS, COLS);
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      tmp_field[i][j] =
          (full_info->info.field[i][j] == ATTACHED_BLOCK ? ATTACHED_BLOCK
                                                         : EMPTY_BLOCK);
      full_info->info.field[i][j] = EMPTY_BLOCK;
      full_info->info.field[i][j] = tmp_field[i][j];
    }
  }
  if (tmp_field) {
    for (int i = 0; i < ROWS; i++) {
      free(tmp_field[i]);
    }
    free(tmp_field);
  }
}

/**
 * Move Figure to the left for a one step
 */
void step_left(void) {
  FULL_INFO;
  full_info->figure.x--;
  (collision_left_check(*full_info) ? full_info->figure.x++
                                    : full_info->figure.x);
  REFRESH_FIGURE;
}

/**
 * Checking the possibility of moving to the left
 * \param full_info a value of structure with game parameters
 */
possible_collision collision_left_check(full_game_info_t full_info) {
  possible_collision collision_check = good_move;
  for (int i = 0; i < MAX_FIG_ROWS; i++) {
    for (int j = 0; j < MAX_FIG_COLS; j++) {
      if (full_info.figure.fig[i][j] == CURRENT_FIGURE_BLOCK &&
          (full_info.info.field[full_info.figure.y + i]
                               [full_info.figure.x + j] == ATTACHED_BLOCK ||
           full_info.figure.x + j < 0)) {
        collision_check = collision_left;
      }
    }
  }
  return collision_check;
}

/**
 * Move Figure to the left for a one step
 */
void step_right(void) {
  FULL_INFO;
  full_info->figure.x++;
  (collision_right_check(*full_info) ? full_info->figure.x--
                                     : full_info->figure.x);
  REFRESH_FIGURE;
}

/**
 * Checking the possibility of moving to the right
 * \param full_info a value of structure with game parameters
 */
possible_collision collision_right_check(full_game_info_t full_info) {
  possible_collision collision_check = good_move;
  for (int i = 0; i < MAX_FIG_ROWS; i++) {
    for (int j = 0; j < MAX_FIG_COLS; j++) {
      if (full_info.figure.fig[i][j] == CURRENT_FIGURE_BLOCK &&
          (full_info.info.field[full_info.figure.y + i]
                               [full_info.figure.x + j] == ATTACHED_BLOCK ||
           full_info.figure.x + j >= COLS)) {
        collision_check = collision_right;
      }
    }
  }
  return collision_check;
}

/**
 * Rotate Figure on a main field
 */
void rotate(void) {
  FULL_INFO;
  if (!collision_down_check(*full_info)) {
    if (full_info->figure.fig_name == 'I')
      rotate_fig_I();
    else if (full_info->figure.fig_name == 'S' ||
             full_info->figure.fig_name == 'Z')
      rotate_fig_S_Z();
    else if (full_info->figure.fig_name != 'O') {
      rotate_fig_left(full_info);
    }
    while (collision_right_check(*full_info)) step_left();
    while (collision_left_check(*full_info)) step_right();
    REFRESH_FIGURE;
  }
}

/**
 * Rotate only 'I' Figure
 */
void rotate_fig_I(void) {
  FULL_INFO;
  while (collision_up_check(*full_info) &&
         full_info->figure.status == horizontal)
    step_down();
  if (full_info->figure.status == horizontal) {
    rotate_fig_I_right(full_info);
  } else if (full_info->figure.status == vertical) {
    rotate_fig_I_left(full_info);
  }
}

/**
 * Rotate only 'I' Figure to the right
 */
void rotate_fig_I_right(full_game_info_t* full_info) {
  for (int i = 0; i < MAX_FIG_COLS / 2; i++) {
    for (int j = i; j < MAX_FIG_COLS - i - 1; j++) {
      int temp = full_info->figure.fig[i][j];
      full_info->figure.fig[i][j] =
          full_info->figure.fig[MAX_FIG_COLS - 1 - j][i];
      full_info->figure.fig[MAX_FIG_COLS - 1 - j][i] =
          full_info->figure.fig[MAX_FIG_COLS - 1 - i][MAX_FIG_COLS - 1 - j];
      full_info->figure.fig[MAX_FIG_COLS - 1 - i][MAX_FIG_COLS - 1 - j] =
          full_info->figure.fig[j][MAX_FIG_COLS - 1 - i];
      full_info->figure.fig[j][MAX_FIG_COLS - 1 - i] = temp;
    }
  }
  full_info->figure.status = vertical;
}

/**
 * Rotate only 'I' Figure to the left
 */
void rotate_fig_I_left(full_game_info_t* full_info) {
  for (int i = 0; i < MAX_FIG_COLS / 2; i++) {
    for (int j = i; j < MAX_FIG_COLS - i - 1; j++) {
      int temp = full_info->figure.fig[i][j];
      full_info->figure.fig[i][j] =
          full_info->figure.fig[j][MAX_FIG_COLS - 1 - i];
      full_info->figure.fig[j][MAX_FIG_COLS - 1 - i] =
          full_info->figure.fig[MAX_FIG_COLS - 1 - i][MAX_FIG_COLS - 1 - j];
      full_info->figure.fig[MAX_FIG_COLS - 1 - i][MAX_FIG_COLS - 1 - j] =
          full_info->figure.fig[MAX_FIG_COLS - 1 - j][i];
      full_info->figure.fig[MAX_FIG_COLS - 1 - j][i] = temp;
    }
  }
  full_info->figure.status = horizontal;
}

/**
 * Rotate only 'S' or 'Z' Figure on the main field
 */
void rotate_fig_S_Z(void) {
  FULL_INFO;
  while (collision_up_check(*full_info) &&
         full_info->figure.status == horizontal)
    step_down();
  if (full_info->figure.status == vertical) {
    rotate_fig_left(full_info);
    full_info->figure.status = horizontal;
  } else if (full_info->figure.status == horizontal) {
    rotate_fig_right(full_info);
    full_info->figure.status = vertical;
  }
}

/**
 * Rotate a Figure to the left
 */
void rotate_fig_left(full_game_info_t* full_info) {
  int temp_f[SQUARE_FIG][SQUARE_FIG];
  for (int i = 0, i_main = 1; i < SQUARE_FIG; i++, i_main++) {
    for (int j = 0; j < SQUARE_FIG; j++) {
      temp_f[i][j] = full_info->figure.fig[i_main][j];
    }
  }
  for (int i = 0; i < SQUARE_FIG / 2; i++) {
    for (int j = i; j < SQUARE_FIG - i - 1; j++) {
      int temp = temp_f[i][j];
      temp_f[i][j] = temp_f[SQUARE_FIG - 1 - j][i];
      temp_f[SQUARE_FIG - 1 - j][i] =
          temp_f[SQUARE_FIG - 1 - i][SQUARE_FIG - 1 - j];
      temp_f[SQUARE_FIG - 1 - i][SQUARE_FIG - 1 - j] =
          temp_f[j][SQUARE_FIG - 1 - i];
      temp_f[j][SQUARE_FIG - 1 - i] = temp;
    }
  }
  for (int i = 0, i_main = 1; i < SQUARE_FIG; i++, i_main++) {
    for (int j = 0; j < SQUARE_FIG; j++) {
      full_info->figure.fig[i_main][j] = temp_f[i][j];
    }
  }
}

/**
 * Rotate a Figure to the right
 */
void rotate_fig_right(full_game_info_t* full_info) {
  int temp_f[SQUARE_FIG][SQUARE_FIG];
  for (int i = 0, i_main = 1; i < SQUARE_FIG; i++, i_main++) {
    for (int j = 0; j < SQUARE_FIG; j++) {
      temp_f[i][j] = full_info->figure.fig[i_main][j];
    }
  }
  for (int i = 0; i < SQUARE_FIG / 2; i++) {
    for (int j = i; j < SQUARE_FIG - i - 1; j++) {
      int temp = temp_f[i][j];
      temp_f[i][j] = temp_f[j][SQUARE_FIG - 1 - i];
      temp_f[j][SQUARE_FIG - 1 - i] =
          temp_f[SQUARE_FIG - 1 - i][SQUARE_FIG - 1 - j];
      temp_f[SQUARE_FIG - 1 - i][SQUARE_FIG - 1 - j] =
          temp_f[SQUARE_FIG - 1 - j][i];
      temp_f[SQUARE_FIG - 1 - j][i] = temp;
    }
  }
  for (int i = 0, i_main = 1; i < SQUARE_FIG; i++, i_main++) {
    for (int j = 0; j < SQUARE_FIG; j++) {
      full_info->figure.fig[i_main][j] = temp_f[i][j];
    }
  }
}

/**
 * Move a Figure or attached blocks on a main field down for a one step
 */
void step_down(void) {
  FULL_INFO;
  (collision_down_check(*full_info) == good_move ? full_info->figure.y++
                                                 : full_info->figure.y);
  REFRESH_FIGURE;
}

/**
 * Checking the upper collision (if there's any non-empty block upper than
 * current Figure) \param full_info a value of structure with game parameters
 */
possible_collision collision_up_check(full_game_info_t full_info) {
  possible_collision collision_check = good_move;
  for (int i = 0; i < MAX_FIG_ROWS; i++) {
    for (int j = 0; j < MAX_FIG_COLS; j++) {
      if (full_info.figure.fig[i][j] == CURRENT_FIGURE_BLOCK &&
          (full_info.info.field[full_info.figure.y + i]
                               [full_info.figure.x + j] == ATTACHED_BLOCK ||
           full_info.figure.y + i <= 0)) {
        collision_check = collision_up;
      }
    }
  }
  return collision_check;
}

/**
 * Checking the 'down' collision (if there's any non-empty block below than
 * current Figure) \param full_info a value of structure with game parameters
 */
possible_collision collision_down_check(full_game_info_t full_info) {
  possible_collision collision_check = good_move;
  for (int i = 0; i < MAX_FIG_ROWS; i++) {
    for (int j = 0; j < MAX_FIG_COLS; j++) {
      if (full_info.figure.fig[i][j] == CURRENT_FIGURE_BLOCK &&
          ((full_info.figure.y + i) == ROWS - 1
               ? 1
               : (full_info.info.field[full_info.figure.y + i + 1]
                                      [full_info.figure.x + j] ==
                  ATTACHED_BLOCK) ||
                     full_info.figure.y + i >= ROWS - 1)) {
        collision_check = collision_down;
      }
    }
  }
  return collision_check;
}

/**
 * Fall a Figure to the bottom of the field or to the first attached Figure
 */
void fall_down(void) {
  FULL_INFO;
  while (!collision_down_check(*full_info)) {
    step_down();
  }
}

/**
 * Сhecking the possibility of lines disappearing and their removal
 */
void check_and_shift_filled_line(void) {
  FULL_INFO;
  int count_lines = 0;
  for (int i = ROWS - 1; i >= 0; i--) {
    int filled = 1;
    for (int j = 0; j < COLS && filled; j++) {
      if (full_info->info.field[i][j] == EMPTY_BLOCK) filled = 0;
    }
    if (filled) {
      step_down_attached_blocks(i);
      count_lines++;
      i++;
    }
  }
  full_info->info.score += updated_score(count_lines);
  if (full_info->info.score > full_info->info.high_score) {
    full_info->info.high_score = full_info->info.score;
    update_high_score();
  }
}

/**
 * Cleaning a main field from any non-empty blocks
 */
void clean_field(void) {
  FULL_INFO;
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      full_info->info.field[i][j] = 0;
    }
  }
}

/**
 * Move only attached blocks down for a count of lines
 * \param line count of steps
 */
void step_down_attached_blocks(int line) {
  FULL_INFO;
  for (int i = line; i > 0; i--) {
    for (int j = 0; j < COLS; j++) {
      full_info->info.field[i][j] = full_info->info.field[i - 1][j];
    }
  }
  for (int j = 0; j < COLS; j++) {
    full_info->info.field[0][j] = 0;
  }
}

/**
 * Adding scores depending on count of filled lines
 * \param lines number of filled lines
 */
int updated_score(int lines) {
  int score = 0;
  switch (lines) {
    case 1:
      score = 100;
      break;
    case 2:
      score = 300;
      break;
    case 3:
      score = 700;
      break;
    case 4:
      score = 1500;
      break;
  }
  return score;
}

/**
 * Get current time. Uses for shifting Figure
 */
long long update_time(void) {
  struct timeval current;
  gettimeofday(&current, NULL);
  return ((long long)current.tv_sec * 1000 + current.tv_usec / 1000);
}

/**
 * Set current speed (in msec) depending on a current game level
 */
void set_speed(void) {
  FULL_INFO;
  full_info->info.speed = (full_info->info.level > 1
                               ? START_SPEED - 100 * (full_info->info.level - 1)
                               : START_SPEED);
}

/**
 * Set current game level depending on a current number of scores
 */
void set_level(void) {
  FULL_INFO;
  full_info->info.level = (full_info->info.score >= LEVEL_STEP
                               ? (full_info->info.score / (int)LEVEL_STEP) + 1
                               : 1);
  if (full_info->info.level > 10) full_info->info.level = 10;
}

/**
 * Get number of high scores from .txt file which stores current high score
 */
void read_high_score(void) {
  FULL_INFO;
  int high_score = -1;
  FILE* f = fopen("./high_score.txt", "r");
  if (f) {
    fscanf(f, "%d", &high_score);
    full_info->info.high_score = high_score;
    fclose(f);
  } else {
    full_info->info.high_score = 0;
  };
}

/**
 * Update (rewrite) new high score
 */
void update_high_score(void) {
  FULL_INFO;
  FILE* f = fopen("./high_score.txt", "w");
  if (f) {
    fprintf(f, "%d", full_info->info.high_score);
    fclose(f);
  }
}